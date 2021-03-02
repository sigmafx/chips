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

void test_playfield_01(void)
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
    TIA_SET_DATA(pins, 0x01);
    pins = tia_tick(&tia, pins); // 2
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF1);
    TIA_SET_DATA(pins, 0x00);
    pins = tia_tick(&tia, pins); // 3
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF2);
    TIA_SET_DATA(pins, 0x00);
    pins = tia_tick(&tia, pins); // 4
    TEST_CHECK(g_colour == 0x00);

    TIA_RESET_CS1(pins);
    for (clock = 5; clock < 68; clock++)
    {
        pins = tia_tick(&tia, pins); // 5 - 67
        TEST_CHECK(g_colour == 0x00);
    }

    for (clock = 68; clock < 148; clock++)
    {
        pins = tia_tick(&tia, pins); // 68 - 147
        if ((clock - 68) / 4 == 0)
        {
            TEST_CHECK(g_colour == 0x04);
        }
        else
        {
            TEST_CHECK(g_colour == 0x02);
        }
    }

    for (clock = 148; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins); // 148 - 227
        if ((clock - 148) / 4 == 0)
        {
            TEST_CHECK(g_colour == 0x04);
        }
        else
        {
            TEST_CHECK(g_colour == 0x02);
        }
    }
}

void test_playfield_02(void)
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
    TIA_SET_DATA(pins, 0x01);
    pins = tia_tick(&tia, pins); // 2
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF1);
    TIA_SET_DATA(pins, 0x00);
    pins = tia_tick(&tia, pins); // 3
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, PF2);
    TIA_SET_DATA(pins, 0x00);
    pins = tia_tick(&tia, pins); // 4
    TEST_CHECK(g_colour == 0x00);
    TIA_SET_ADDR(pins, CTRLPF);
    TIA_SET_DATA(pins, 0x01);
    pins = tia_tick(&tia, pins); // 5
    TEST_CHECK(g_colour == 0x00);

    TIA_RESET_CS1(pins);
    for (clock = 6; clock < 68; clock++)
    {
        pins = tia_tick(&tia, pins); // 6 - 67
        TEST_CHECK(g_colour == 0x00);
    }

    for (clock = 68; clock < 148; clock++)
    {
        pins = tia_tick(&tia, pins); // 68 - 147
        if ((clock - 68) / 4 == 0)
        {
            TEST_CHECK(g_colour == 0x04);
        }
        else
        {
            TEST_CHECK(g_colour == 0x02);
        }
    }

    for (clock = 148; clock < 228; clock++)
    {
        pins = tia_tick(&tia, pins); // 148 - 227
        if ((clock - 148) / 4 == 19)
        {
            TEST_CHECK(g_colour == 0x04);
        }
        else
        {
            TEST_CHECK(g_colour == 0x02);
        }
    }
}

TEST_LIST = {
    { "test_register_write",    test_register_write },
    { "test_register_read",     test_register_read },
    { "test_WSYNC_RDY",         test_WSYNC_RDY },
    { "test_HSYNC_RDY",         test_HSYNC_RDY },
    { "test_playfield_01",      test_playfield_01 },
    { "test_playfield_02",      test_playfield_02},
    { NULL, NULL }
};
