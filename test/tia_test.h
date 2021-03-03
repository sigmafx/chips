#define CHIPS_IMPL
#include "tia.h"

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