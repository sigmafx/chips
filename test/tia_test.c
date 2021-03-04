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

void test_ball_01_size(void)
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

void test_ball_02_wrap(void)
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

TEST_LIST = {
    { "test_register_write",            test_register_write },
    { "test_register_read",             test_register_read },
    { "test_WSYNC_RDY",                 test_WSYNC_RDY },
    { "test_HSYNC_RDY",                 test_HSYNC_RDY },
    { "test_playfield_01_duplicate",    test_playfield_01_duplicate },
    { "test_playfield_02_reflect",      test_playfield_02_reflect },
    { "test_playfield_03_score",        test_playfield_03_score },
    { "test_ball_01_RESBL",             test_ball_01_RESBL },
    { "test_ball_01_size",              test_ball_01_size },
    { "test_ball_02_wrap",              test_ball_02_wrap },
    { NULL, NULL }
};
