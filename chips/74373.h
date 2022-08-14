/*
    ## Quad 2 Input NAND
    ## Emulated Pins
    *************************************
    *           +-----------+           *
    *   OC  --->|           |     Vcc   *
    *   1Q  <---|           |---> 8Q    *
    *   1D  --->|           |<--- 8D    *
    *   2D  --->|   74373   |<--- 7D    *
    *   2Q  <---|           |---> 7Q    *
    *   3Q  <---|           |---> 6Q    *
    *   3D  --->|           |<--- 6D    *
    *   4D  --->|           |<--- 5D    *
    *   4Q  <---|           |---> 5Q    *
    *  GND      |           |<--- C     *
    *           +-----------+           *
    *************************************
*/

#include <stdint.h>

#define TTL74373_OC     (1ULL<<0)
#define TTL74373_1Q     (1Ull<<1)
#define TTL74373_1D     (1Ull<<2)
#define TTL74373_2D     (1Ull<<3)
#define TTL74373_2Q     (1Ull<<4)
#define TTL74373_3Q     (1Ull<<5)
#define TTL74373_3D     (1Ull<<6)
#define TTL74373_4D     (1Ull<<7)
#define TTL74373_4Q     (1Ull<<8)
#define TTL74373_C      (1Ull<<10)
#define TTL74373_5Q     (1Ull<<11)
#define TTL74373_5D     (1Ull<<12)
#define TTL74373_6D     (1Ull<<13)
#define TTL74373_6Q     (1Ull<<14)
#define TTL74373_7Q     (1Ull<<15)
#define TTL74373_7D     (1Ull<<16)
#define TTL74373_8D     (1Ull<<17)
#define TTL74373_8Q     (1Ull<<18)

// Gate 1
#define TTL74373_SET_A1(p) ((p)|=TTL74373_A1,p=TTL74373_Act_Gate1(p))
#define TTL74373_RESET_A1(p) ((p)&=~TTL74373_A1,p=TTL74373_Act_Gate1(p))
#define TTL74373_SET_B1(p) ((p)|=TTL74373_B1,p=TTL74373_Act_Gate1(p))
#define TTL74373_RESET_B1(p) ((p)&=~TTL74373_B1,p=TTL74373_Act_Gate1(p))
#define TTL74373_GET_Y1(p) (p&TTL74373_Y1)

#define TTL74373_GET_A1(p) ((p)&TTL74373_A1)
#define TTL74373_GET_B1(p) ((p)&TTL74373_B1)
#define TTL74373_SET_Y1(p) (p|=TTL74373_Y1)
#define TTL74373_RESET_Y1(p) (p&=~TTL74373_Y1)

// Gate 2
#define TTL74373_SET_A2(p) ((p)|=TTL74373_A2,p=TTL74373_Act_Gate2(p))
#define TTL74373_RESET_A2(p) ((p)&=~TTL74373_A2,p=TTL74373_Act_Gate2(p))
#define TTL74373_SET_B2(p) ((p)|=TTL74373_B2,p=TTL74373_Act_Gate2(p))
#define TTL74373_RESET_B2(p) ((p)&=~TTL74373_B2,p=TTL74373_Act_Gate2(p))
#define TTL74373_GET_Y2(p) (p&TTL74373_Y2)

#define TTL74373_GET_A2(p) ((p)&TTL74373_A2)
#define TTL74373_GET_B2(p) ((p)&TTL74373_B2)
#define TTL74373_SET_Y2(p) (p|=TTL74373_Y2)
#define TTL74373_RESET_Y2(p) (p&=~TTL74373_Y2)

// Gate 3
#define TTL74373_SET_A3(p) ((p)|=TTL74373_A3,p=TTL74373_Act_Gate3(p))
#define TTL74373_RESET_A3(p) ((p)&=~TTL74373_A3,p=TTL74373_Act_Gate3(p))
#define TTL74373_SET_B3(p) ((p)|=TTL74373_B3,p=TTL74373_Act_Gate3(p))
#define TTL74373_RESET_B3(p) ((p)&=~TTL74373_B3,p=TTL74373_Act_Gate3(p))
#define TTL74373_GET_Y3(p) (p&TTL74373_Y3)

#define TTL74373_GET_A3(p) ((p)&TTL74373_A3)
#define TTL74373_GET_B3(p) ((p)&TTL74373_B3)
#define TTL74373_SET_Y3(p) (p|=TTL74373_Y3)
#define TTL74373_RESET_Y3(p) (p&=~TTL74373_Y3)

// Gate 4
#define TTL74373_SET_A4(p) ((p)|=TTL74373_A4,p=TTL74373_Act_Gate4(p))
#define TTL74373_RESET_A4(p) ((p)&=~TTL74373_A4,p=TTL74373_Act_Gate4(p))
#define TTL74373_SET_B4(p) ((p)|=TTL74373_B4,p=TTL74373_Act_Gate4(p))
#define TTL74373_RESET_B4(p) ((p)&=~TTL74373_B4,p=TTL74373_Act_Gate4(p))
#define TTL74373_GET_Y4(p) (p&TTL74373_Y4)

#define TTL74373_GET_A4(p) ((p)&TTL74373_A4)
#define TTL74373_GET_B4(p) ((p)&TTL74373_B4)
#define TTL74373_SET_Y4(p) (p|=TTL74373_Y4)
#define TTL74373_RESET_Y4(p) (p&=~TTL74373_Y4)

inline uint64_t TTL74373_Act_Gate1(uint64_t pins)
{
    (TTL74373_GET_A1(pins) && TTL74373_GET_B1(pins)) ? TTL74373_RESET_Y1(pins) : TTL74373_SET_Y1(pins);

    return pins;
}

inline uint64_t TTL74373_Act_Gate2(uint64_t pins)
{
    (TTL74373_GET_A2(pins) && TTL74373_GET_B2(pins)) ? TTL74373_RESET_Y2(pins) : TTL74373_SET_Y2(pins);

    return pins;
}

inline uint64_t TTL74373_Act_Gate3(uint64_t pins)
{
    (TTL74373_GET_A3(pins) && TTL74373_GET_B3(pins)) ? TTL74373_RESET_Y3(pins) : TTL74373_SET_Y3(pins);

    return pins;
}

inline uint64_t TTL74373_Act_Gate4(uint64_t pins)
{
    (TTL74373_GET_A4(pins) && TTL74373_GET_B4(pins)) ? TTL74373_RESET_Y4(pins) : TTL74373_SET_Y4(pins);

    return pins;
}
