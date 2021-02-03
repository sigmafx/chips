#include "acutest.h"
#define CHIPS_IMPL
#include "m6532.h"

void test_AllMemoryCanBeZero(void)
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

void test_AllMemoryCanBeSet(void)
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

void test_NoMemoryReadWhenCS1Reset(void)
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

void test_NoMemoryReadWhenCS2Set(void)
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

void test_NoMemoryReadWhenCS1ResetCS2Set(void)
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

void test_PortA_DDR_ReadWrite(void)
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
    } while (++ddr != 0x00);
}

void test_PortB_DDR_ReadWrite(void)
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
    } while (++ddr != 0x00);
}

void test_PortA_DR_ReadWrite(void)
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
    } while (++dr != 0x00);
}

void test_PortB_DR_ReadWrite(void)
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
    } while (++dr != 0x00);
}

void test_PortA_Output(void)
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

        data = M6532_GET_PA(pins);
        TEST_CHECK(data == dr);
    } while (++dr != 0x00);
}

void test_PortB_Output(void)
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

        data = M6532_GET_PB(pins);
        TEST_CHECK(data == dr);
    } while (++dr != 0x00);
}

void test_PortA_Input(void)
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_SET_RS(pins);

    // Port A DDR all input
    M6532_RESET_RW(pins);
    M6532_SET_ADDR(pins, 0b0000001);
    M6532_SET_DATA(pins, 0x00);
    pins = m6532_tick(&riot, pins);

    // Port A DR
    M6532_SET_ADDR(pins, 0b0000000);
    M6532_SET_RW(pins);
    uint8_t input = 0x00;
    do
    {
        M6532_SET_PA(pins, input);

        pins = m6532_tick(&riot, pins);

        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == input);
        TEST_CHECK(M6532_GET_PA(pins) == input);
    } while (++input != 0x00);
}

void test_PortB_Input(void)
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_SET_RS(pins);

    // Port B DDR all input
    M6532_RESET_RW(pins);
    M6532_SET_ADDR(pins, 0b0000011);
    M6532_SET_DATA(pins, 0x00);
    pins = m6532_tick(&riot, pins);

    // Port B DR
    M6532_SET_ADDR(pins, 0b0000010);
    M6532_SET_RW(pins);
    uint8_t input = 0x00;
    do
    {
        M6532_SET_PB(pins, input);

        pins = m6532_tick(&riot, pins);

        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == input);
        TEST_CHECK(M6532_GET_PB(pins) == input);
    } while (++input != 0x00);
}

void test_PortA_InputOutput(void)
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_SET_RS(pins);
    M6532_RESET_RW(pins);

    // Port A DDR - hi nibble input / lo nibble output
    M6532_SET_ADDR(pins, 0b0000001);
    M6532_SET_DATA(pins, 0x0F);
    pins = m6532_tick(&riot, pins);

    // Port A DR - All output bits set
    M6532_SET_ADDR(pins, 0b0000000);
    M6532_SET_DATA(pins, 0xFF);
    pins = m6532_tick(&riot, pins);

    M6532_SET_RW(pins);
    uint8_t input = 0x00;
    do
    {
        M6532_SET_PA(pins, input);

        pins = m6532_tick(&riot, pins);

        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == ((input & 0xF0) | 0x0F));
        TEST_CHECK(M6532_GET_PA(pins) == ((input & 0xF0) | 0x0F));
    } while (++input != 0x00);
}

void test_PortB_InputOutput(void)
{
    m6532_t riot;
    uint64_t pins = 0ULL;
    uint8_t data;

    // Prepare
    m6532_init(&riot);
    M6532_SET_CS1(pins);
    M6532_RESET_CS2(pins);
    M6532_SET_RS(pins);
    M6532_RESET_RW(pins);

    // Port A DDR - hi nibble input / lo nibble output
    M6532_SET_ADDR(pins, 0b0000011);
    M6532_SET_DATA(pins, 0x0F);
    pins = m6532_tick(&riot, pins);

    // Port A DR - All output bits set
    M6532_SET_ADDR(pins, 0b0000010);
    M6532_SET_DATA(pins, 0xFF);
    pins = m6532_tick(&riot, pins);

    M6532_SET_RW(pins);
    uint8_t input = 0x00;
    do
    {
        M6532_SET_PB(pins, input);

        pins = m6532_tick(&riot, pins);

        data = M6532_GET_DATA(pins);
        TEST_CHECK(data == ((input & 0xF0) | 0x0F));
        TEST_CHECK(M6532_GET_PB(pins) == ((input & 0xF0) | 0x0F));
    } while (++input != 0x00);
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
    { "test_PortB_DR_ReadWrite", test_PortB_DR_ReadWrite },
    { "test_PortA_Output", test_PortA_Output },
    { "test_PortB_Output", test_PortB_Output},
    { "test_PortA_Input", test_PortA_Input},
    { "test_PortB_Input", test_PortB_Input},
    { "test_PortA_InputOutput", test_PortA_InputOutput},
    { "test_PortB_InputOutput", test_PortB_InputOutput},
    { NULL, NULL }
};
