/*
    ## Quad 2 Input NAND
    ## Emulated Pins
    *************************************
    *           +-----------+           *
    *   A1  --->|           |           *
    *   B1  --->|           |<--- B4    *
    *   Y1  <---|           |<--- A4    *
    *   A2  --->|   7400    |---> Y4    *
    *   B2  --->|           |<--- B3    *
    *   Y2  <---|           |<--- A3    *
    *           |           |---> Y3    *
    *           +-----------+           *
    *************************************
*/

#include <stdint.h>

#define TTL7400_A1     (1ULL<<0)
#define TTL7400_B1     (1Ull<<1)
#define TTL7400_Y1     (1Ull<<2)
#define TTL7400_A2     (1Ull<<3)
#define TTL7400_B2     (1Ull<<4)
#define TTL7400_Y2     (1Ull<<5)
#define TTL7400_A3     (1Ull<<7)
#define TTL7400_B3     (1Ull<<8)
#define TTL7400_Y3     (1Ull<<9)
#define TTL7400_A4     (1Ull<<10)
#define TTL7400_B4     (1Ull<<11)
#define TTL7400_Y4     (1Ull<<12)

// Gate 1
#define TTL7400_SET_A1(p) ((p)|=TTL7400_A1,p=TTL7400_Act_Gate1(p))
#define TTL7400_RESET_A1(p) ((p)&=~TTL7400_A1,p=TTL7400_Act_Gate1(p))
#define TTL7400_SET_B1(p) ((p)|=TTL7400_B1,p=TTL7400_Act_Gate1(p))
#define TTL7400_RESET_B1(p) ((p)&=~TTL7400_B1,p=TTL7400_Act_Gate1(p))
#define TTL7400_GET_Y1(p) (p&TTL7400_Y1)

#define TTL7400_GET_A1(p) ((p)&TTL7400_A1)
#define TTL7400_GET_B1(p) ((p)&TTL7400_B1)
#define TTL7400_SET_Y1(p) (p|=TTL7400_Y1)
#define TTL7400_RESET_Y1(p) (p&=~TTL7400_Y1)

// Gate 2
#define TTL7400_SET_A2(p) ((p)|=TTL7400_A2,p=TTL7400_Act_Gate2(p))
#define TTL7400_RESET_A2(p) ((p)&=~TTL7400_A2,p=TTL7400_Act_Gate2(p))
#define TTL7400_SET_B2(p) ((p)|=TTL7400_B2,p=TTL7400_Act_Gate2(p))
#define TTL7400_RESET_B2(p) ((p)&=~TTL7400_B2,p=TTL7400_Act_Gate2(p))
#define TTL7400_GET_Y2(p) (p&TTL7400_Y2)

#define TTL7400_GET_A2(p) ((p)&TTL7400_A2)
#define TTL7400_GET_B2(p) ((p)&TTL7400_B2)
#define TTL7400_SET_Y2(p) (p|=TTL7400_Y2)
#define TTL7400_RESET_Y2(p) (p&=~TTL7400_Y2)

// Gate 3
#define TTL7400_SET_A3(p) ((p)|=TTL7400_A3,p=TTL7400_Act_Gate3(p))
#define TTL7400_RESET_A3(p) ((p)&=~TTL7400_A3,p=TTL7400_Act_Gate3(p))
#define TTL7400_SET_B3(p) ((p)|=TTL7400_B3,p=TTL7400_Act_Gate3(p))
#define TTL7400_RESET_B3(p) ((p)&=~TTL7400_B3,p=TTL7400_Act_Gate3(p))
#define TTL7400_GET_Y3(p) (p&TTL7400_Y3)

#define TTL7400_GET_A3(p) ((p)&TTL7400_A3)
#define TTL7400_GET_B3(p) ((p)&TTL7400_B3)
#define TTL7400_SET_Y3(p) (p|=TTL7400_Y3)
#define TTL7400_RESET_Y3(p) (p&=~TTL7400_Y3)

// Gate 4
#define TTL7400_SET_A4(p) ((p)|=TTL7400_A4,p=TTL7400_Act_Gate4(p))
#define TTL7400_RESET_A4(p) ((p)&=~TTL7400_A4,p=TTL7400_Act_Gate4(p))
#define TTL7400_SET_B4(p) ((p)|=TTL7400_B4,p=TTL7400_Act_Gate4(p))
#define TTL7400_RESET_B4(p) ((p)&=~TTL7400_B4,p=TTL7400_Act_Gate4(p))
#define TTL7400_GET_Y4(p) (p&TTL7400_Y4)

#define TTL7400_GET_A4(p) ((p)&TTL7400_A4)
#define TTL7400_GET_B4(p) ((p)&TTL7400_B4)
#define TTL7400_SET_Y4(p) (p|=TTL7400_Y4)
#define TTL7400_RESET_Y4(p) (p&=~TTL7400_Y4)

inline uint64_t TTL7400_Act_Gate1(uint64_t pins)
{
    (TTL7400_GET_A1(pins) && TTL7400_GET_B1(pins)) ? TTL7400_RESET_Y1(pins) : TTL7400_SET_Y1(pins);

    return pins;
}

inline uint64_t TTL7400_Act_Gate2(uint64_t pins)
{
    (TTL7400_GET_A2(pins) && TTL7400_GET_B2(pins)) ? TTL7400_RESET_Y2(pins) : TTL7400_SET_Y2(pins);

    return pins;
}

inline uint64_t TTL7400_Act_Gate3(uint64_t pins)
{
    (TTL7400_GET_A3(pins) && TTL7400_GET_B3(pins)) ? TTL7400_RESET_Y3(pins) : TTL7400_SET_Y3(pins);

    return pins;
}

inline uint64_t TTL7400_Act_Gate4(uint64_t pins)
{
    (TTL7400_GET_A4(pins) && TTL7400_GET_B4(pins)) ? TTL7400_RESET_Y4(pins) : TTL7400_SET_Y4(pins);

    return pins;
}
