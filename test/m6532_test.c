#include "acutest.h"
#define CHIPS_IMPL
#include "m6532.h"

void test_AllMemoryCanBeZero()
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;
    uint8_t addr;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_RESET_RS(pins);

    // Check all memory can be zero
    for (addr = 0; addr < 0x80; addr++)
    {
        M6532_RESET_RW(pins);
        M6532_SET_DATA(pins, 0x00);
        M6532_SET_ADDR(pins, addr);
        pins = m6532_tick(&riot, pins);

        M6532_SET_RW(pins);
        M6532_SET_DATA(pins, 0xFF);
        pins = m6532_tick(&riot, pins);
        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == 0x00);
    }
}

void test_AllMemoryCanBeSet()
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;
    uint8_t addr;


    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_RESET_RS(pins);

    // Check all memory can be set
    M6532_RESET_RW(pins);
    M6532_RESET_RS(pins);
    for (addr = 0; addr < 0x80; addr++)
    {
        M6532_RESET_RW(pins);
        M6532_SET_DATA(pins, (addr | 0x80));
        M6532_SET_ADDR(pins, addr);
        pins = m6532_tick(&riot, pins);

        M6532_SET_RW(pins);
        M6532_SET_DATA(pins, 0x00);
        pins = m6532_tick(&riot, pins);
        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == (addr | 0x80));
    }
}

void test_NoMemoryReadWhenCS1Reset()
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;
    uint8_t addr;

    // Prepare
    m6532_init(&riot);
    M6532_RESET_CS2(pins);
    M6532_RESET_RS(pins);

    for (addr = 0; addr < 0x80; addr++)
    {
        M6532_SET_CS1(pins);
        M6532_SET_DATA(pins, 0xFF);
        M6532_SET_ADDR(pins, addr);
        M6532_RESET_RW(pins);
        pins = m6532_tick(&riot, pins);

        M6532_RESET_CS1(pins);
        M6532_SET_DATA(pins, 0x00);
        M6532_SET_RW(pins);
        pins = m6532_tick(&riot, pins);
        data = M6532_GET_DATA(pins);
        TEST_CHECK(data != 0xFF);
    }
}

void test_NoMemoryReadWhenCS2Set()
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;
    uint8_t addr;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_RS(pins);

    for (addr = 0; addr < 0x80; addr++)
    {
        M6532_RESET_CS2(pins);
        M6532_SET_DATA(pins, 0xFF);
        M6532_SET_ADDR(pins, addr);
        M6532_RESET_RW(pins);
        pins = m6532_tick(&riot, pins);

        M6532_SET_CS2(pins);
        M6532_SET_DATA(pins, 0x00);
        M6532_SET_RW(pins);
        pins = m6532_tick(&riot, pins);
        data = M6532_GET_DATA(pins);
        TEST_CHECK(data != 0xFF);
    }
}

void test_NoMemoryReadWhenCS1ResetCS2Set()
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;
    uint8_t addr;

    // Prepare
    m6532_init(&riot);
    M6532_RESET_RS(pins);

    for (addr = 0; addr < 0x80; addr++)
    {
        M6532_SET_CS1(pins);
        M6532_RESET_CS2(pins);
        M6532_SET_DATA(pins, 0xFF);
        M6532_SET_ADDR(pins, addr);
        M6532_RESET_RW(pins);
        pins = m6532_tick(&riot, pins);

        M6532_RESET_CS1(pins);
        M6532_SET_CS2(pins);
        M6532_SET_DATA(pins, 0x00);
        M6532_SET_RW(pins);
        pins = m6532_tick(&riot, pins);
        data = M6532_GET_DATA(pins);
        TEST_CHECK(data != 0xFF);
    }
}

void test_PortA_DDR_ReadWrite()
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_SET_RS(pins);

    // Port A DDR
    M6532_SET_ADDR(pins, 0b0000001);
    uint8_t ddr = 0x00;
    do
    {
        M6532_RESET_RW(pins);
        M6532_SET_DATA(pins, ddr);
        pins = m6532_tick(&riot, pins);

        M6532_SET_RW(pins);
        M6532_SET_DATA(pins, 0x00);
        pins = m6532_tick(&riot, pins);

        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == ddr);

        ddr++;
    } while (ddr != 0x00);
}

void test_PortB_DDR_ReadWrite()
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_SET_RS(pins);

    // Port B DDR
    M6532_SET_ADDR(pins, 0b0000011);
    uint8_t ddr = 0x00;
    do
    {
        M6532_RESET_RW(pins);
        M6532_SET_DATA(pins, ddr);
        pins = m6532_tick(&riot, pins);

        M6532_SET_RW(pins);
        M6532_SET_DATA(pins, 0x00);
        pins = m6532_tick(&riot, pins);

        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == ddr);

        ddr++;
    } while (ddr != 0x00);
}

void test_PortA_DR_ReadWrite()
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_SET_RS(pins);

    // Port A DDR all output
    M6532_RESET_RW(pins);
    M6532_SET_ADDR(pins, 0b0000001);
    M6532_SET_DATA(pins, 0xFF);
    pins = m6532_tick(&riot, pins);

    // Port A DR
    M6532_SET_ADDR(pins, 0b0000000);
    uint8_t dr = 0x00;
    do
    {
        M6532_RESET_RW(pins);
        M6532_SET_DATA(pins, dr);
        pins = m6532_tick(&riot, pins);

        M6532_SET_RW(pins);
        M6532_SET_DATA(pins, 0x00);
        pins = m6532_tick(&riot, pins);

        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == dr);

        dr++;
    } while (dr != 0x00);
}

void test_PortB_DR_ReadWrite()
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_SET_RS(pins);

    // Port B DDR all output
    M6532_RESET_RW(pins);
    M6532_SET_ADDR(pins, 0b0000011);
    M6532_SET_DATA(pins, 0xFF);
    pins = m6532_tick(&riot, pins);

    // Port B DR
    M6532_SET_ADDR(pins, 0b0000010);
    uint8_t dr = 0x00;
    do
    {
        M6532_RESET_RW(pins);
        M6532_SET_DATA(pins, dr);
        pins = m6532_tick(&riot, pins);

        M6532_SET_RW(pins);
        M6532_SET_DATA(pins, 0x00);
        pins = m6532_tick(&riot, pins);

        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == dr);

        dr++;
    } while (dr != 0x00);
}

TEST_LIST = {
    { "test_AllMemoryCanBeZero",    test_AllMemoryCanBeZero },
    { "test_AllMemoryCanBeSet",     test_AllMemoryCanBeSet },
    { "test_NoMemoryReadWhenCS1Reset", test_NoMemoryReadWhenCS1Reset },
    { "test_NoMemoryReadWhenCS2Set", test_NoMemoryReadWhenCS2Set },
    { "test_NoMemoryReadWhenCS1ResetCS2Set", test_NoMemoryReadWhenCS1ResetCS2Set },
    { "test_PortA_DDR_ReadWrite", test_PortA_DDR_ReadWrite },
    { "test_PortB_DDR_ReadWrite", test_PortB_DDR_ReadWrite},
    { "test_PortA_DR_ReadWrite", test_PortA_DR_ReadWrite },
    { "test_PortB_DR_ReadWrite", test_PortB_DR_ReadWrite},
    { NULL, NULL }
};
