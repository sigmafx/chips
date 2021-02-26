#include "acutest.h"
#define CHIPS_IMPL
#include "tia.h"

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
    uint8_t regWrite[] = {
        VSYNC,
        VBLANK,
        NUSIZ0,
        NUSIZ1,
        COLUP0,
        COLUP1,
        COLUPF,
        COLUBK,
        CTRLPF,
        REFP0,
        REFP1,
        PF0,
        PF1,
        PF2,
        AUDC0,
        AUDC1,
        AUDF0,
        AUDF1,
        AUDV0,
        AUDV1,
        GRP0,
        GRP1,
        ENAM0,
        ENAM1,
        ENABL,
        HMP0,
        HMP1,
        HMM0,
        HMM1,
        HMBL,
        VDELP0,
        VDELP1,
        VDELBL,
        RESMP0,
        RESMP1
    };

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
    uint8_t regRead[] = {
        CXM0P,
        CXM1P,
        CXP0FB,
        CXP1FB,
        CXM0FB,
        CXM1FB,
        CXBLPF,
        CXPPMM,
        INPT0,
        INPT1,
        INPT2,
        INPT3,
        INPT4,
        INPT5
    };

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

TEST_LIST = {
    { "test_register_write",    test_register_write },
    { "test_register_read",     test_register_read },
    { NULL, NULL }
};
