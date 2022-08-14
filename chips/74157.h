/*
    ## Quad 2 Input Multiplexer
    ## Emulated Pins
    *************************************
    *           +-----^-----+           *
    *     S --->|           |     Vcc   *
    *   I0a --->|           |<--- EE    *
    *   I1a --->|           |<--- I0c   *
    *    Za <---|   74157   |<--- I1c   *
    *   I0b --->|           |---> Zc    *
    *   I1b --->|           |<--- I0d   *
    *    Zb <---|           |<--- I1d   *
    *   GND     |           |---> Zd    *
    *           +-----------+           *
    *************************************
*/

#include <stdint.h>

#define TTL74157_S       (1ULL<<0)
#define TTL74157_I0a     (1Ull<<1)
#define TTL74157_I1a     (1Ull<<2)
#define TTL74157_Za      (1Ull<<3)
#define TTL74157_I0b     (1Ull<<4)
#define TTL74157_I1b     (1Ull<<5)
#define TTL74157_Zb      (1Ull<<6)
#define TTL74157_Zd      (1Ull<<8)
#define TTL74157_I1d     (1Ull<<9)
#define TTL74157_I0d     (1Ull<<10)
#define TTL74157_Zc      (1Ull<<11)
#define TTL74157_I1c     (1Ull<<12)
#define TTL74157_I0c     (1Ull<<13)
#define TTL74157_EE      (1Ull<<14)

// Control
#define TTL74157_SET_S(p) ((p)|=TTL74157_S,(p)=TTL74157_Act_Gate1((p)))
#define TTL74157_RESET_S(p) ((p)&=~TTL74157_S,(p)=TTL74157_Act_Gate1((p)))
#define TTL74157_SET_EE(p) ((p)|=TTL74157_EE,(p)=TTL74157_Act_Gate1((p)))
#define TTL74157_RESET_EE(p) ((p)&=~TTL74157_EE,(p)=TTL74157_Act_Gate1((p)))

// Gate 1
#define TTL74157_SET_I0a(p) ((p)|=TTL74157_I0a,(p)=TTL74157_Act_Gate1((p)))
#define TTL74157_RESET_I0a(p) ((p)&=~TTL74157_I0a,(p)=TTL74157_Act_Gate1((p)))
#define TTL74157_SET_I1a(p) ((p)|=TTL74157_I1a,(p)=TTL74157_Act_Gate1((p)))
#define TTL74157_RESET_I1a(p) ((p)&=~TTL74157_I1a,(p)=TTL74157_Act_Gate1((p)))
#define TTL74157_GET_Za(p) (p&TTL74157_Za)

#define TTL74157_GET_S(p) ((p)&TTL74157_S)
#define TTL74157_GET_EE(p) ((p)&TTL74157_EE)
#define TTL74157_GET_I0a(p) ((p)&TTL74157_I0a)
#define TTL74157_GET_I1a(p) ((p)&TTL74157_I1a)
#define TTL74157_SET_Za(p) (p|=TTL74157_Za)
#define TTL74157_RESET_Za(p) (p&=~TTL74157_Za)

inline uint64_t TTL74157_Act_Gate1(uint64_t pins)
{
    TTL74157_GET_EE(pins) ? TTL74157_RESET_Za(pins) :
    TTL74157_GET_S(pins) ?  (TTL74157_GET_I1a(pins) ? TTL74157_SET_Za(pins) : TTL74157_RESET_Za(pins)) :
                            (TTL74157_GET_I0a(pins) ? TTL74157_SET_Za(pins) : TTL74157_RESET_Za(pins));
    return pins;
}
