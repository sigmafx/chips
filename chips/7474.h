/*
    ## Dual D-Type Flip Flop
    ## Emulated Pins
    *************************************
    *           +-----------+           *
    *  1CLR --->|           |           *
    *    1D --->|           |<--- 2CLR  *
    *  1CLK --->|           |<--- 2D    *
    *  1PRE --->|   7400    |<--- 2CLK  *
    *    1Q <---|           |<--- 2PRE  *
    *   1QQ <---|           |---> 2Q    *
    *           |           |---> 2QQ   *
    *           +-----------+           *
    *************************************
*/

#include <stdint.h>

#define TTL7474_1CLR    (1ULL<<0)
#define TTL7474_1D      (1Ull<<1)
#define TTL7474_1CLK    (1Ull<<2)
#define TTL7474_1PRE    (1Ull<<3)
#define TTL7474_1Q      (1Ull<<4)
#define TTL7474_1QQ     (1Ull<<5)
#define TTL7474_2QQ     (1Ull<<7)
#define TTL7474_2Q      (1Ull<<8)
#define TTL7474_2PRE    (1Ull<<9)
#define TTL7474_2CLK    (1Ull<<10)
#define TTL7474_2D      (1Ull<<11)
#define TTL7474_2CLR    (1Ull<<12)

// Flip Flop 1
#define TTL7474_SET_1CLR(t,p) ((p)|=TTL7474_1CLR,(p)=TTL7474_Act_Gate1((t),(p)))
#define TTL7474_RESET_1CLR(t,p) ((p)&=~TTL7474_1CLR,(p)=TTL7474_Act_Gate1((t),(p)))
#define TTL7474_SET_1D(t,p) ((p)|=TTL7474_1D,(p)=TTL7474_Act_Gate1((t),(p)))
#define TTL7474_RESET_1D(t,p) ((p)&=~TTL7474_1D,(p)=TTL7474_Act_Gate1((t),(p)))
#define TTL7474_SET_1CLK(t,p) ((p)|=TTL7474_1CLK,(p)=TTL7474_Act_Gate1((t),(p)))
#define TTL7474_RESET_1CLK(t,p) ((p)&=~TTL7474_1CLK,(p)=TTL7474_Act_Gate1((t),(p)))
#define TTL7474_SET_1PRE(t,p) ((p)|=TTL7474_1PRE,(p)=TTL7474_Act_Gate1((t),(p)))
#define TTL7474_RESET_1PRE(t,p) ((p)&=~TTL7474_1PRE,(p)=TTL7474_Act_Gate1((t),(p)))
#define TTL7474_GET_1Q(p) (p&TTL7474_1Q)
#define TTL7474_GET_1QQ(p) (p&TTL7474_1QQ)

#define TTL7474_GET_1CLR(p) (p&TTL7474_1CLR)
#define TTL7474_GET_1D(p) (p&TTL7474_1D)
#define TTL7474_GET_1CLK(p) (p&TTL7474_1CLK)
#define TTL7474_GET_1PRE(p) (p&TTL7474_1PRE)
#define TTL7474_SET_1Q(p) (p&TTL7474_1Q)
#define TTL7474_RESET_1Q(p) (p&=~TTL7474_1Q)
#define TTL7474_SET_1QQ(p) (p&TTL7474_1QQ)
#define TTL7474_RESET_1QQ(p) (p&=~TTL7474_1QQ)

typedef struct
{
    uint64_t pinsPrev;
} TTL7474_t;


uint64_t TTL7474_Init(TTL7474_t* ttl7474)
{
    ttl7474->pinsPrev = 0x00000000;

    return 0x00000000;
}

inline uint64_t TTL7474_Act_Gate1(TTL7474_t* ttl7474, uint64_t pins)
{
    if(TTL7474_GET_1PRE(pins) == 0) {
        if(TTL7474_GET_1CLR(pins) == 0) {
            TTL7474_SET_1Q(pins);
            TTL7474_SET_1QQ(pins);
        }
        else
        {
            TTL7474_SET_1Q(pins);
            TTL7474_RESET_1QQ(pins);
        }
    }
    else if(TTL7474_GET_1CLR(pins) == 0)
    {
            TTL7474_RESET_1Q(pins);
            TTL7474_SET_1QQ(pins);
    }
    else if(TTL7474_GET_1CLK(pins) == 0)
    {
        TTL7474_GET_1Q(ttl7474->pinsPrev) ? TTL7474_SET_1Q(pins) : TTL7474_RESET_1Q(pins);
        TTL7474_GET_1QQ(ttl7474->pinsPrev) ? TTL7474_SET_1QQ(pins) : TTL7474_RESET_1QQ(pins);
    }
    else
    {
        if(TTL7474_GET_1CLK(ttl7474->pinsPrev) == 0)
        {
            TTL7474_GET_1D(pins) ? TTL7474_SET_1Q(pins) : TTL7474_RESET_1Q(pins);
            TTL7474_GET_1D(pins) ? TTL7474_RESET_1QQ(pins) : TTL7474_SET_1QQ(pins);
        }
    }

    ttl7474->pinsPrev = pins;
    return pins;
}
