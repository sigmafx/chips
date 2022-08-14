/*
    ## 4 Bit Counter
    ## Emulated Pins
    *************************************
    *           +-----------+           *
    *  CP1  --->|           |<--- CP0   *
    *  MR1  --->|           |     NC    *
    *  MR2  --->|           |---> Q0    *
    *   NC      |   7493    |---> Q3    *
    *  VCC      |           |     GND   *
    *   NC      |           |---> Q1    *
    *   NC      |           |---> Q2    *
    *           +-----------+           *
    *************************************
*/

#include <stdint.h>

#define TTL7493_CP1     (1ULL<<0)
#define TTL7493_MR1     (1Ull<<1)
#define TTL7493_MR2     (1Ull<<2)
#define TTL7493_Q2      (1Ull<<8)
#define TTL7493_Q1      (1Ull<<9)
#define TTL7493_Q3      (1Ull<<11)
#define TTL7493_Q0      (1Ull<<12)
#define TTL7493_CP0     (1Ull<<14)

#define TTL7493_SET_CP1(t,p) ((p)|=TTL7493_CP1,p=TTL7493_Act(t,p))
#define TTL7493_RESET_CP1(t,p) ((p)&=~TTL7493_CP1,p=TTL7493_Act(t,p))
#define TTL7493_SET_MR1(t,p) ((p)|=TTL7493_MR11,p=TTL7493_Act(t,p))
#define TTL7493_RESET_MR11(t,p) ((p)&=~TTL7493_MR11,p=TTL7493_Act(t,p))
#define TTL7493_SET_MR2(t,p) ((p)|=TTL7493_MR2,p=TTL7493_Act(t,p))
#define TTL7493_RESET_MR2(t,p) ((p)&=~TTL7493_MR2,p=TTL7493_Act(t,p))
#define TTL7493_SET_CP0(t,p) ((p)|=TTL7493_CP0,p=TTL7493_Act(t,p))
#define TTL7493_RESET_CP0(t,p) ((p)&=~TTL7493_CP0,p=TTL7493_Act(t,p))
#define TTL7493_GET_Q0(p) (p&TTL7493_Q0)
#define TTL7493_GET_Q1(p) (p&TTL7493_Q1)
#define TTL7493_GET_Q2(p) (p&TTL7493_Q2)
#define TTL7493_GET_Q3(p) (p&TTL7493_Q3)

#define TTL7493_GET_CP1(p) ((p)&TTL7493_CP1)
#define TTL7493_GET_MR1(p) ((p)&TTL7493_MR1)
#define TTL7493_GET_MR2(p) ((p)&TTL7493_MR2)
#define TTL7493_GET_CP0(p) ((p)&TTL7493_CP0)
#define TTL7493_SET_Q0(p) (p|=TTL7493_Q0)
#define TTL7493_RESET_Q0(p) (p&=~TTL7493_Q0)
#define TTL7493_SET_Q1(p) (p|=TTL7493_Q1)
#define TTL7493_RESET_Q1(p) (p&=~TTL7493_Q1)
#define TTL7493_SET_Q2(p) (p|=TTL7493_Q2)
#define TTL7493_RESET_Q2(p) (p&=~TTL7493_Q2)
#define TTL7493_SET_Q3(p) (p|=TTL7493_Q3)
#define TTL7493_RESET_Q3(p) (p&=~TTL7493_Q3)

typedef struct
{
    uint8_t q;
    uint64_t pinsPrev;
} TTL7493_t;


uint64_t TTL7493_Init(TTL7493_t* ttl7493)
{
    ttl7493->q = 0x00;
    ttl7493->pinsPrev = 0x00000000;
    return 0x00000000;
}

inline uint64_t TTL7493_Act(TTL7493_t* t, uint64_t pins)
{
    if (TTL7493_GET_MR1(pins) && TTL7493_GET_MR2(pins))
    {
        t->q = 0x00;
    }
    else
    {
        if (!TTL7493_GET_CP0(pins) && TTL7493_GET_CP0(t->pinsPrev))
        {
            t-> q = ((t->q & 0x01) ? t->q & 0xFE : t->q | 0x01);
        }

        if (!TTL7493_GET_CP1(pins) && TTL7493_GET_CP1(t->pinsPrev))
        {
            t->q += 0x02;
            t->q &= 0x0F;
        }
    }

    t->q & 0x01 ? TTL7493_SET_Q0(pins) : TTL7493_RESET_Q0(pins);
    t->q & 0x02 ? TTL7493_SET_Q1(pins) : TTL7493_RESET_Q1(pins);
    t->q & 0x04 ? TTL7493_SET_Q2(pins) : TTL7493_RESET_Q2(pins);
    t->q & 0x08 ? TTL7493_SET_Q3(pins) : TTL7493_RESET_Q3(pins);
    t->pinsPrev = pins;

    return pins;
}
