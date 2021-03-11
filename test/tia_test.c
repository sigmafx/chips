#include "acutest.h"
#include "tia_test.h"

uint8_t g_colour;

void colour_tick(uint8_t colour)
{
    g_colour = colour;
}

uint64_t init_with_colour(tia_t* tia)
{
    uint64_t pins = tia_init(tia, NTSC, colour_tick, NULL, NULL);

    TIA_SET_CS1(pins);

    return pins;
}

uint64_t init_write(tia_t* tia, uint8_t addr, uint8_t data)
{
    uint64_t pins = tia_init(tia, NTSC, NULL, NULL, NULL);

    TIA_SET_CS1(pins);
    TIA_RESET_RW(pins);
    TIA_SET_ADDR(pins, addr);
    TIA_SET_DATA(pins, data);

    return pins;
}

uint64_t init_read(tia_t* tia, uint8_t addr)
{
    uint64_t pins = tia_init(tia, NTSC, NULL, NULL, NULL);

    TIA_SET_CS1(pins);
    TIA_SET_RW(pins);
    TIA_SET_ADDR(pins, addr);

    return pins;
}

void test_register_write(void)
{
    tia_t tia;
    uint64_t pins;

    int idxReg;
    for (idxReg = 0; idxReg < sizeof(regWrite) / sizeof(regWrite[0]); idxReg++)
    {
        pins = init_write(&tia, regWrite[idxReg], 0xFF);
        pins = tia_tick(&tia, pins);
        TEST_CHECK(tia.regWrite[regWrite[idxReg]] == 0xFF);
    }
}

void test_register_read(void)
{
    tia_t tia;
    uint64_t pins;

    int idxReg;
    int idxMax = sizeof(regRead) / sizeof(regRead[0]);
    for (idxReg = 0; idxReg < idxMax; idxReg++)
    {
        pins = init_read(&tia, regRead[idxReg]);
        tia.regRead[regRead[idxReg]] = 0xFF;
        pins = tia_tick(&tia, pins);
        TEST_CHECK(TIA_GET_DATA(pins) == 0xFF);
    }
}

void test_WSYNC_RDY(void)
{
    tia_t tia;
    uint64_t pins;

    pins = init_write(&tia, WSYNC, 0xFF);
    pins = tia_tick(&tia, pins);
    TEST_CHECK(TIA_GET_RDY(pins));
}

void test_HSYNC_RDY(void)
{
    tia_t tia;
    uint64_t pins;

    pins = init_write(&tia, WSYNC, 0xFF);

    int clock;
    for (clock = 0; clock < 227; clock++)
    {
        pins = tia_tick(&tia, pins);
        TEST_CHECK(TIA_GET_RDY(pins));
    }

    pins = tia_tick(&tia, pins);
    TEST_CHECK(!TIA_GET_RDY(pins));
}

void test_playfield_01_duplicate(void)
{
    tia_t tia;
    uint64_t pins;
    uint16_t clock;

    pins = init_with_colour(&tia);

    TIA_RESET_RW(pins);
    TIA_SET_ADDR(pins, COLUBK);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 0
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, COLUPF);
    TIA_SET_DATA(pins, 0x04);
    pins = tia_tick(&tia, pins); // 1
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF0);
    TIA_SET_DATA(pins, 0x20);
    pins = tia_tick(&tia, pins); // 2
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF1);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 3
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF2);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 4
    TEST_CHECK(g_colour == 0x00);

    TIA_RESET_CS1(pins);
    for (clock = 5; clock < 68; clock++)
    {
        pins = tia_tick(&tia, pins); // 5 - 67
        TEST_CHECK(g_colour == 0x00);
    }

    for (clock = 68; clock < 228; clock++)
    {
        uint8_t bkClock = (clock - 68) / 4;

        pins = tia_tick(&tia, pins); // 68 - 227
        switch (bkClock)
        {
            case 1:
            case 10:
            case 13:
            case 21:
            case 30:
            case 33:
                TEST_CHECK(g_colour == 0x04);
                break;

            default:
                TEST_CHECK(g_colour == 0x02);
                break;
        }
    }
}

void test_playfield_02_reflect(void)
{
    tia_t tia;
    uint64_t pins;
    uint16_t clock;

    pins = init_with_colour(&tia);

    TIA_RESET_RW(pins);
    TIA_SET_ADDR(pins, COLUBK);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 0
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, COLUPF);
    TIA_SET_DATA(pins, 0x04);
    pins = tia_tick(&tia, pins); // 1
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x01);
    pins = tia_tick(&tia, pins); // 2
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF0);
    TIA_SET_DATA(pins, 0x20);
    pins = tia_tick(&tia, pins); // 3
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF1);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 4
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF2);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 5
    TEST_CHECK(g_colour == 0x00);

    TIA_RESET_CS1(pins);
    for (clock = 6; clock < 68; clock++)
    {
        pins = tia_tick(&tia, pins); // 5 - 67
        TEST_CHECK(g_colour == 0x00);
    }

    for (clock = 68; clock < 228; clock++)
    {
        uint8_t bkClock = (clock - 68) / 4;

        pins = tia_tick(&tia, pins); // 68 - 227
        switch (bkClock)
        {
        case 1:
        case 10:
        case 13:
        case 26:
        case 29:
        case 38:
            TEST_CHECK(g_colour == 0x04);
            break;

        default:
            TEST_CHECK(g_colour == 0x02);
            break;
        }
    }
}

void test_playfield_03_score(void)
{
    tia_t tia;
    uint64_t pins;
    uint16_t clock;

    pins = init_with_colour(&tia);

    TIA_RESET_RW(pins);
    TIA_SET_ADDR(pins, COLUBK);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 0
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, COLUPF);
    TIA_SET_DATA(pins, 0x04);
    pins = tia_tick(&tia, pins); // 1
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, COLUP0);
    TIA_SET_DATA(pins, 0x06);
    pins = tia_tick(&tia, pins); // 2
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, COLUP1);
    TIA_SET_DATA(pins, 0x08);
    pins = tia_tick(&tia, pins); // 3
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 4
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF0);
    TIA_SET_DATA(pins, 0x20);
    pins = tia_tick(&tia, pins); // 5
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF1);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 6
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF2);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 7
    TEST_CHECK(g_colour == 0x00);

    TIA_RESET_CS1(pins);
    for (clock = 8; clock < 68; clock++)
    {
        pins = tia_tick(&tia, pins); // 8 - 67
        TEST_CHECK(g_colour == 0x00);
    }

    for (clock = 68; clock < 228; clock++)
    {
        uint8_t bkClock = (clock - 68) / 4;

        pins = tia_tick(&tia, pins); // 68 - 227
        switch (bkClock)
        {
            case 1:
            case 10:
            case 13:
                TEST_CHECK(g_colour == 0x06);
                break;

            case 21:
            case 30:
            case 33:
                TEST_CHECK(g_colour == 0x08);
                break;

            default:
                TEST_CHECK(g_colour == 0x02);
                break;
        }
    }
}

void test_ball_01_RESBL(void)
{
    tia_t tia;
    uint64_t pins;
    uint16_t clock;

    pins = init_with_colour(&tia);

    TIA_RESET_RW(pins);
    TIA_SET_ADDR(pins, RESBL);
    for (clock = 0; clock < 228; clock++)
    {
        TIA_SET_DATA(pins, 0xFF);
        pins = tia_tick(&tia, pins);

        TEST_CHECK(tia.posBL == (clock < 68 ? 0 : clock - 68));
    }
}

void test_ball_02_size(void)
{
    tia_t tia;
    uint64_t pins;
    uint16_t clock;

    pins = init_with_colour(&tia);

    TIA_RESET_RW(pins);

    TIA_SET_ADDR(pins, COLUPF);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 0
    TIA_SET_ADDR(pins, ENABL);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 1

    // Width 1
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x00);
    pins = tia_tick(&tia, pins); // 2

    for (clock = 3; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins);
        switch (clock)
        {
        case 68:
                TEST_CHECK(g_colour == 0x02);
                break;

            default:
                TEST_CHECK(g_colour == 0x00);
                break;
        }
    }

    // Width 2
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x10);
    pins = tia_tick(&tia, pins); // 0

    for (clock = 1; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins);
        switch (clock)
        {
        case 68:
        case 69:
            TEST_CHECK(g_colour == 0x02);
            break;

        default:
            TEST_CHECK(g_colour == 0x00);
            break;
        }
    }

    // Width 4
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x20);
    pins = tia_tick(&tia, pins); // 0

    for (clock = 1; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins);
        switch (clock)
        {
        case 68:
        case 69:
        case 70:
        case 71:
            TEST_CHECK(g_colour == 0x02);
            break;

        default:
            TEST_CHECK(g_colour == 0x00);
            break;
        }
    }

    // Width 8
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x30);
    pins = tia_tick(&tia, pins); // 0

    for (clock = 1; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins);
        switch (clock)
        {
        case 68:
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
            TEST_CHECK(g_colour == 0x02);
            break;

        default:
            TEST_CHECK(g_colour == 0x00);
            break;
        }
    }
}

void test_ball_03_wrap(void)
{
    tia_t tia;
    uint64_t pins;
    uint16_t clock;

    pins = init_with_colour(&tia);

    TIA_RESET_RW(pins);

    TIA_SET_ADDR(pins, COLUPF);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 0
    TIA_SET_ADDR(pins, ENABL);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 1

    // Width 2
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x10);
    pins = tia_tick(&tia, pins); // 2

    TIA_SET_ADDR(pins, RESBL);
    TIA_SET_DATA(pins, 0xFF);

    for (clock = 3; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins);
    }

    TEST_CHECK(g_colour == 0x02);

    TIA_RESET_CS1(pins);

    for (clock = 0; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins);
        switch (clock)
        {
        case 68:
        case 227:
            TEST_CHECK(g_colour == 0x02);
            break;

        default:
            TEST_CHECK(g_colour == 0x00);
            break;
        }
    }

    TIA_SET_CS1(pins);

    // Width 3
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x20);
    pins = tia_tick(&tia, pins); // 0

    TIA_RESET_CS1(pins);

    for (clock = 1; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins);
        switch (clock)
        {
        case 68:
        case 69:
        case 70:
        case 227:
            TEST_CHECK(g_colour == 0x02);
            break;

        default:
            TEST_CHECK(g_colour == 0x00);
            if (g_colour != 0x00) printf("%d\n", clock);
            break;
        }
    }

    TIA_SET_CS1(pins);

    // Width 4
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x30);
    pins = tia_tick(&tia, pins); // 0

    TIA_RESET_CS1(pins);

    for (clock = 1; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins);
        switch (clock)
        {
        case 68:
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 227:
            TEST_CHECK(g_colour == 0x02);
            break;

        default:
            TEST_CHECK(g_colour == 0x00);
            break;
        }
    }
}

void test_ball_04_delay(void)
{
    tia_t tia;
    uint64_t pins;
    uint16_t clock;

    pins = init_with_colour(&tia);

    TIA_RESET_RW(pins);

    TIA_SET_ADDR(pins, COLUPF);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 0
    TIA_SET_ADDR(pins, ENABL);
    TIA_SET_DATA(pins, 0x02);
    pins = tia_tick(&tia, pins); // 1
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x00);
    pins = tia_tick(&tia, pins); // 2
    TIA_SET_ADDR(pins, VDELBL);
    TIA_SET_DATA(pins, 0x01);
    pins = tia_tick(&tia, pins); // 3

    for (clock = 4; clock < 228; clock++)
    {
        switch (clock)
        {
        case 68:
            TIA_SET_ADDR(pins, RESBL);
            TIA_SET_DATA(pins, 0xFF);
            TIA_SET_CS1(pins);
            pins = tia_tick(&tia, pins);
            TEST_CHECK(g_colour == 0x00);
            break;

        default:
            TIA_RESET_CS1(pins);
            pins = tia_tick(&tia, pins);
            TEST_CHECK(g_colour == 0x00);
            break;
        }
            
    }

    for (clock = 0; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins);
        switch (clock)
        {
        case 68:
            TEST_CHECK(g_colour == 0x02);
            break;

        default:
            TEST_CHECK(g_colour == 0x00);
            break;
        }
    }
}

void test_shiftreg_01_reg8(void)
{
    tia_shiftreg_t shiftreg;
    uint8_t clock;

    tia_shiftreg_init(&shiftreg, 8);

    for (clock = 0; clock < 10; clock++)
    {
        bool bit = tia_shiftreg_tick(&shiftreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_shiftreg_enable(&shiftreg, 1, forward, 0xF0);
            break;

        case 5:
        case 6:
        case 7:
        case 8:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_shiftreg_02_delay8(void)
{
    tia_shiftreg_t shiftreg;
    uint8_t clock;

    tia_shiftreg_init(&shiftreg, 8);
    tia_shiftreg_enable(&shiftreg, 1, forward, 0xF0);

    for (clock = 0; clock < 16; clock++)
    {
        bool bit = tia_shiftreg_tick(&shiftreg, true);

        switch (clock)
        {
        case 12:
        case 13:
        case 14:
        case 15:
            TEST_CHECK(bit);
            break;

        case 7:
            tia_shiftreg_enable(&shiftreg, 1, forward, 0xF0);
        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_shiftreg_03_reg1(void)
{
    tia_shiftreg_t shiftreg;
    uint8_t clock;

    tia_shiftreg_init(&shiftreg, 1);

    for (clock = 0; clock < 10; clock++)
    {
        bool bit = tia_shiftreg_tick(&shiftreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_shiftreg_enable(&shiftreg, 1, forward, 0x01);
            break;

        case 1:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_shiftreg_04_delay1(void)
{
    tia_shiftreg_t shiftreg;
    uint8_t clock;

    tia_shiftreg_init(&shiftreg, 1);
    tia_shiftreg_enable(&shiftreg, 1, forward, 0x01);

    for (clock = 0; clock < 16; clock++)
    {
        bool bit = tia_shiftreg_tick(&shiftreg, true);

        switch (clock)
        {
        case 8:
            TEST_CHECK(bit);
            break;

        case 7:
            tia_shiftreg_enable(&shiftreg, 1, forward, 0x01);
        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_shiftreg_05_reg8_reverse(void)
{
    tia_shiftreg_t shiftreg;
    uint8_t clock;

    tia_shiftreg_init(&shiftreg, 8);

    for (clock = 0; clock < 10; clock++)
    {
        bool bit = tia_shiftreg_tick(&shiftreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_shiftreg_enable(&shiftreg, 1, reverse, 0xF0);
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_shiftreg_06_delay8_reverse(void)
{
    tia_shiftreg_t shiftreg;
    uint8_t clock;

    tia_shiftreg_init(&shiftreg, 8);
    tia_shiftreg_enable(&shiftreg, 1, reverse, 0xF0);

    for (clock = 0; clock < 16; clock++)
    {
        bool bit = tia_shiftreg_tick(&shiftreg, true);

        switch (clock)
        {
        case 8:
        case 9:
        case 10:
        case 11:
            TEST_CHECK(bit);
            break;

        case 7:
            tia_shiftreg_enable(&shiftreg, 1, reverse, 0xF0);
        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_shiftreg_07_reg4_divider(void)
{
    tia_shiftreg_t shiftreg;
    uint8_t clock;

    tia_shiftreg_init(&shiftreg, 4);

    for (clock = 0; clock < 20; clock++)
    {
        bool bit = tia_shiftreg_tick(&shiftreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_shiftreg_enable(&shiftreg, 4, forward, 0b1010);
            break;

        case 5:
        case 6:
        case 7:
        case 8:
        case 13:
        case 14:
        case 15:
        case 16:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_shiftreg_07_reg4_divider_reverse(void)
{
    tia_shiftreg_t shiftreg;
    uint8_t clock;

    tia_shiftreg_init(&shiftreg, 4);

    for (clock = 0; clock < 20; clock++)
    {
        bool bit = tia_shiftreg_tick(&shiftreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_shiftreg_enable(&shiftreg, 4, reverse, 0b1010);
            break;

        case 1:
        case 2:
        case 3:
        case 4:
        case 9:
        case 10:
        case 11:
        case 12:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_bitreg_01_width1(void)
{
    tia_bitreg_t bitreg;
    uint8_t clock;

    tia_bitreg_init(&bitreg);

    for (clock = 0; clock < 20; clock++)
    {
        bool bit = tia_bitreg_tick(&bitreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_bitreg_start(&bitreg, 0, 0x01, 0);
            break;

        case 1:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_bitreg_02_width2(void)
{
    tia_bitreg_t bitreg;
    uint8_t clock;

    tia_bitreg_init(&bitreg);

    for (clock = 0; clock < 20; clock++)
    {
        bool bit = tia_bitreg_tick(&bitreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_bitreg_start(&bitreg, 1, 0x01, 0);
            break;

        case 1:
        case 2:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_bitreg_03_width4(void)
{
    tia_bitreg_t bitreg;
    uint8_t clock;

    tia_bitreg_init(&bitreg);

    for (clock = 0; clock < 20; clock++)
    {
        bool bit = tia_bitreg_tick(&bitreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_bitreg_start(&bitreg, 2, 0x01, 0);
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_bitreg_04_width8(void)
{
    tia_bitreg_t bitreg;
    uint8_t clock;

    tia_bitreg_init(&bitreg);

    for (clock = 0; clock < 20; clock++)
    {
        bool bit = tia_bitreg_tick(&bitreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_bitreg_start(&bitreg, 3, 0x01, 0);
            break;

        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_bitreg_05_width8_ffwd6(void)
{
    tia_bitreg_t bitreg;
    uint8_t clock;

    tia_bitreg_init(&bitreg);

    for (clock = 0; clock < 20; clock++)
    {
        bool bit = tia_bitreg_tick(&bitreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_bitreg_start(&bitreg, 3, 0x01, 6);
            break;

        case 1:
        case 2:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_bitreg_06_width8_ffwd7(void)
{
    tia_bitreg_t bitreg;
    uint8_t clock;

    tia_bitreg_init(&bitreg);

    for (clock = 0; clock < 20; clock++)
    {
        bool bit = tia_bitreg_tick(&bitreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_bitreg_start(&bitreg, 3, 0x01, 7);
            break;

        case 1:
            TEST_CHECK(bit);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

void test_bitreg_07_width8_ffwd8(void)
{
    tia_bitreg_t bitreg;
    uint8_t clock;

    tia_bitreg_init(&bitreg);

    for (clock = 0; clock < 20; clock++)
    {
        bool bit = tia_bitreg_tick(&bitreg, false);

        switch (clock)
        {
        case 0:
            TEST_CHECK(!bit);
            tia_bitreg_start(&bitreg, 3, 0x01, 8);
            break;

        default:
            TEST_CHECK(!bit);
            break;
        }
    }
}

TEST_LIST = {
    { "test_register_write",            test_register_write },
    { "test_register_read",             test_register_read },
    { "test_WSYNC_RDY",                 test_WSYNC_RDY },
    { "test_HSYNC_RDY",                 test_HSYNC_RDY },
    { "test_playfield_01_duplicate",    test_playfield_01_duplicate },
    { "test_playfield_02_reflect",      test_playfield_02_reflect },
    { "test_playfield_03_score",        test_playfield_03_score },
    { "test_ball_01_RESBL",             test_ball_01_RESBL },
    { "test_ball_02_size",              test_ball_02_size },
    { "test_ball_03_wrap",              test_ball_03_wrap },
    { "test_ball_04_delay",             test_ball_04_delay },
    { "test_shiftreg_01_reg8",          test_shiftreg_01_reg8 },
    { "test_shiftreg_02_delay8",        test_shiftreg_02_delay8 },
    { "test_shiftreg_03_reg1",          test_shiftreg_03_reg1 },
    { "test_shiftreg_04_delay1",        test_shiftreg_04_delay1 },
    { "test_shiftreg_05_reg8_reverse",  test_shiftreg_05_reg8_reverse },
    { "test_shiftreg_06_delay8_reverse",test_shiftreg_06_delay8_reverse },
    { "test_shiftreg_07_reg4_divider",  test_shiftreg_07_reg4_divider },
    { "test_shiftreg_07_reg4_divider_reverse", test_shiftreg_07_reg4_divider_reverse },
    { "test_bitreg_01_width1",          test_bitreg_01_width1 },
    { "test_bitreg_02_width2",          test_bitreg_02_width2 },
    { "test_bitreg_03_width4",          test_bitreg_03_width4 },
    { "test_bitreg_04_width8",          test_bitreg_04_width8 },
    { "test_bitreg_05_width8_ffwd6",    test_bitreg_05_width8_ffwd6 },
    { "test_bitreg_06_width8_ffwd7",    test_bitreg_06_width8_ffwd7 },
    { "test_bitreg_07_width8_ffwd8",    test_bitreg_07_width8_ffwd8 },
    { NULL, NULL }
};
