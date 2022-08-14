/*
    ## Hex Inverter
    ## Emulated Pins
    *************************************
    *           +-----------+           *
    *   A1  --->|           |           *
    *   Y1  <---|           |<--- A6    *
    *   A2  --->|           |---> Y6    *
    *   Y2  <---|   7404    |<--- A5    *
    *   A3  --->|           |---> Y5    *
    *   Y3  <---|           |<--- A4    *
    *           |           |---> Y4    *
    *           +-----------+           *
    *************************************
*/

#include <stdint.h>

#define TTL7404_A1     (1ULL<<0)
#define TTL7404_Y1     (1Ull<<1)
#define TTL7404_A2     (1Ull<<2)
#define TTL7404_Y2     (1Ull<<3)
#define TTL7404_A3     (1Ull<<4)
#define TTL7404_Y3     (1Ull<<5)
#define TTL7404_Y4     (1Ull<<7)
#define TTL7404_A4     (1Ull<<8)
#define TTL7404_Y5     (1Ull<<9)
#define TTL7404_A5     (1Ull<<10)
#define TTL7404_Y6     (1Ull<<11)
#define TTL7404_A6     (1Ull<<12)

// Gate 1
#define TTL7404_SET_A1(p) ((p)|=TTL7404_A1,p=TTL7404_Act_Gate1(p))
#define TTL7404_RESET_A1(p) ((p)&=~TTL7404_A1,p=TTL7404_Act_Gate1(p))
#define TTL7404_GET_Y1(p) (p&TTL7404_Y1)

#define TTL7404_GET_A1(p) ((p)&TTL7404_A1)
#define TTL7404_SET_Y1(p) (p|=TTL7404_Y1)
#define TTL7404_RESET_Y1(p) (p&=~TTL7404_Y1)

// Gate 2
#define TTL7404_SET_A2(p) ((p)|=TTL7404_A2,p=TTL7404_Act_Gate2(p))
#define TTL7404_RESET_A2(p) ((p)&=~TTL7404_A2,p=TTL7404_Act_Gate2(p))
#define TTL7404_GET_Y2(p) (p&TTL7404_Y2)

#define TTL7404_GET_A2(p) ((p)&TTL7404_A2)
#define TTL7404_SET_Y2(p) (p|=TTL7404_Y2)
#define TTL7404_RESET_Y2(p) (p&=~TTL7404_Y2)

// Gate 3
#define TTL7404_SET_A3(p) ((p)|=TTL7404_A3,p=TTL7404_Act_Gate3(p))
#define TTL7404_RESET_A3(p) ((p)&=~TTL7404_A3,p=TTL7404_Act_Gate3(p))
#define TTL7404_GET_Y3(p) (p&TTL7404_Y3)

#define TTL7404_GET_A3(p) ((p)&TTL7404_A3)
#define TTL7404_SET_Y3(p) (p|=TTL7404_Y3)
#define TTL7404_RESET_Y3(p) (p&=~TTL7404_Y3)

// Gate 4
#define TTL7404_SET_A4(p) ((p)|=TTL7404_A4,p=TTL7404_Act_Gate4(p))
#define TTL7404_RESET_A4(p) ((p)&=~TTL7404_A4,p=TTL7404_Act_Gate4(p))
#define TTL7404_GET_Y4(p) (p&TTL7404_Y4)

#define TTL7404_GET_A4(p) ((p)&TTL7404_A4)
#define TTL7404_SET_Y4(p) (p|=TTL7404_Y4)
#define TTL7404_RESET_Y4(p) (p&=~TTL7404_Y4)

// Gate 5
#define TTL7404_SET_A5(p) ((p)|=TTL7404_A5,p=TTL7404_Act_Gate5(p))
#define TTL7404_RESET_A5(p) ((p)&=~TTL7404_A5,p=TTL7404_Act_Gate5(p))
#define TTL7404_GET_Y5(p) (p&TTL7404_Y5)

#define TTL7404_GET_A5(p) ((p)&TTL7404_A5)
#define TTL7404_SET_Y5(p) (p|=TTL7404_Y5)
#define TTL7404_RESET_Y5(p) (p&=~TTL7404_Y5)

// Gate 6
#define TTL7404_SET_A6(p) ((p)|=TTL7404_A6,p=TTL7404_Act_Gate6(p))
#define TTL7404_RESET_A6(p) ((p)&=~TTL7404_A6,p=TTL7404_Act_Gate6(p))
#define TTL7404_GET_Y6(p) (p&TTL7404_Y6)

#define TTL7404_GET_A6(p) ((p)&TTL7404_A6)
#define TTL7404_SET_Y6(p) (p|=TTL7404_Y6)
#define TTL7404_RESET_Y6(p) (p&=~TTL7404_Y6)

inline uint64_t TTL7404_Act_Gate1(uint64_t pins)
{
    TTL7404_GET_A1(pins) ? TTL7404_RESET_Y1(pins) : TTL7404_SET_Y1(pins);

    return pins;
}

inline uint64_t TTL7404_Act_Gate2(uint64_t pins)
{
    TTL7404_GET_A2(pins) ? TTL7404_RESET_Y2(pins) : TTL7404_SET_Y2(pins);

    return pins;
}

inline uint64_t TTL7404_Act_Gate3(uint64_t pins)
{
    TTL7404_GET_A3(pins) ? TTL7404_RESET_Y3(pins) : TTL7404_SET_Y3(pins);

    return pins;
}

inline uint64_t TTL7404_Act_Gate4(uint64_t pins)
{
    TTL7404_GET_A4(pins) ? TTL7404_RESET_Y4(pins) : TTL7404_SET_Y4(pins);

    return pins;
}

inline uint64_t TTL7404_Act_Gate5(uint64_t pins)
{
    TTL7404_GET_A5(pins) ? TTL7404_RESET_Y5(pins) : TTL7404_SET_Y5(pins);

    return pins;
}

inline uint64_t TTL7404_Act_Gate6(uint64_t pins)
{
    TTL7404_GET_A6(pins) ? TTL7404_RESET_Y6(pins) : TTL7404_SET_Y6(pins);

    return pins;
}
