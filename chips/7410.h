/*
    ## Triple 3 Input NAND
    ## Emulated Pins
    *************************************
    *           +-----------+           *
    *   A1  --->|           |           *
    *   Y1  <---|           |<--- A6    *
    *   A2  --->|           |---> Y6    *
    *   Y2  <---|   7410    |<--- A5    *
    *   A3  --->|           |---> Y5    *
    *   Y3  <---|           |<--- A4    *
    *           |           |---> Y4    *
    *           +-----------+           *
    *************************************
*/

#include <stdint.h>

#define TTL7410_A1     (1ULL<<0)
#define TTL7410_B1     (1ULL<<1)
#define TTL7410_A2     (1ULL<<2)
#define TTL7410_B2     (1ULL<<3)
#define TTL7410_C2     (1ULL<<4)
#define TTL7410_Y2     (1ULL<<5)
#define TTL7410_Y3     (1ULL<<7)
#define TTL7410_C3     (1ULL<<8)
#define TTL7410_B3     (1ULL<<9)
#define TTL7410_A3     (1ULL<<10)
#define TTL7410_Y1     (1ULL<<11)
#define TTL7410_C1     (1ULL<<12)

// Gate 1
#define TTL7410_SET_A1(p) ((p)|=TTL7410_A1,(p)=TTL7410_Act_Gate1((p)))
#define TTL7410_RESET_A1(p) ((p)&=~TTL7410_A1,(p)=TTL7410_Act_Gate1((p)))
#define TTL7410_SET_B1(p) ((p)|=TTL7410_B1,(p)=TTL7410_Act_Gate1((p)))
#define TTL7410_RESET_B1(p) ((p)&=~TTL7410_B1,(p)=TTL7410_Act_Gate1((p)))
#define TTL7410_SET_C1(p) ((p)|=TTL7410_C1,(p)=TTL7410_Act_Gate1((p)))
#define TTL7410_RESET_C1(p) ((p)&=~TTL7410_C1,(p)=TTL7410_Act_Gate1((p)))
#define TTL7410_GET_Y1(p) (p&TTL7410_Y1)

#define TTL7410_GET_A1(p) ((p)&TTL7410_A1)
#define TTL7410_GET_B1(p) ((p)&TTL7410_B1)
#define TTL7410_GET_C1(p) ((p)&TTL7410_C1)
#define TTL7410_SET_Y1(p) (p|=TTL7410_Y1)
#define TTL7410_RESET_Y1(p) (p&=~TTL7410_Y1)

// Gate 2
#define TTL7410_SET_A2(p) ((p)|=TTL7410_A2,(p)=TTL7410_Act_Gate2((p)))
#define TTL7410_RESET_A2(p) ((p)&=~TTL7410_A2,(p)=TTL7410_Act_Gate2((p)))
#define TTL7410_SET_B2(p) ((p)|=TTL7410_B2,(p)=TTL7410_Act_Gate2((p)))
#define TTL7410_RESET_B2(p) ((p)&=~TTL7410_B2,(p)=TTL7410_Act_Gate2((p)))
#define TTL7410_SET_C2(p) ((p)|=TTL7410_C2,(p)=TTL7410_Act_Gate2((p)))
#define TTL7410_RESET_C2(p) ((p)&=~TTL7410_C2,(p)=TTL7410_Act_Gate2((p)))
#define TTL7410_GET_Y2(p) (p&TTL7410_Y2)

#define TTL7410_GET_A2(p) ((p)&TTL7410_A2)
#define TTL7410_GET_B2(p) ((p)&TTL7410_B2)
#define TTL7410_GET_C2(p) ((p)&TTL7410_C2)
#define TTL7410_SET_Y2(p) (p|=TTL7410_Y2)
#define TTL7410_RESET_Y2(p) (p&=~TTL7410_Y2)

// Gate 3
#define TTL7410_SET_A3(p) ((p)|=TTL7410_A3,(p)=TTL7410_Act_Gate3((p)))
#define TTL7410_RESET_A3(p) ((p)&=~TTL7410_A3,(p)=TTL7410_Act_Gate3((p)))
#define TTL7410_SET_B3(p) ((p)|=TTL7410_B3,(p)=TTL7410_Act_Gate3((p)))
#define TTL7410_RESET_B3(p) ((p)&=~TTL7410_B3,(p)=TTL7410_Act_Gate3((p)))
#define TTL7410_SET_C3(p) ((p)|=TTL7410_C3,(p)=TTL7410_Act_Gate3((p)))
#define TTL7410_RESET_C3(p) ((p)&=~TTL7410_C3,(p)=TTL7410_Act_Gate3((p)))
#define TTL7410_GET_Y3(p) (p&TTL7410_Y3)

#define TTL7410_GET_A3(p) ((p)&TTL7410_A3)
#define TTL7410_GET_B3(p) ((p)&TTL7410_B3)
#define TTL7410_GET_C3(p) ((p)&TTL7410_C3)
#define TTL7410_SET_Y3(p) (p|=TTL7410_Y3)
#define TTL7410_RESET_Y3(p) (p&=~TTL7410_Y3)

inline uint64_t TTL7410_Act_Gate1(uint64_t pins)
{
    (TTL7410_GET_A1(pins) | TTL7410_GET_B1(pins) | TTL7410_GET_C1(pins)) ? TTL7410_RESET_Y1(pins) : TTL7410_SET_Y1(pins);
    return pins;
}

inline uint64_t TTL7410_Act_Gate2(uint64_t pins)
{
    (TTL7410_GET_A2(pins) | TTL7410_GET_B2(pins) | TTL7410_GET_C2(pins)) ? TTL7410_RESET_Y2(pins) : TTL7410_SET_Y2(pins);
    return pins;
}

inline uint64_t TTL7410_Act_Gate3(uint64_t pins)
{
    (TTL7410_GET_A3(pins) | TTL7410_GET_B3(pins) | TTL7410_GET_C3(pins)) ? TTL7410_RESET_Y3(pins) : TTL7410_SET_Y3(pins);
    return pins;
}
