/*
    ## Emulated Pins
    *************************************
    *           +-----------+           *
    *   D0  <---|           |           *
    *        ...|           |           *
    *   D7  <---|           |           *
    *           |    cart   |           *
    *   A0  --->|           |<--- CS    *
    *        ...|           |           *
    *   A11 --->|           |           *
    *           +-----------+           *
    *************************************
*/

#include <stdint.h>
#include <memory.h>

#define CART_A0        (1ULL<<0)
#define CART_A1        (1ULL<<1)
#define CART_A2        (1ULL<<2)
#define CART_A3        (1ULL<<3)
#define CART_A4        (1ULL<<4)
#define CART_A5        (1ULL<<5)
#define CART_A6        (1ULL<<6)
#define CART_A7        (1ULL<<7)
#define CART_A8        (1ULL<<8)
#define CART_A9        (1ULL<<9)
#define CART_A10       (1ULL<<10)
#define CART_A11       (1ULL<<11)
#define CART_A12       (1ULL<<12)
#define CART_AB_PINS   (CART_A0|CART_A1|CART_A2|CART_A3|CART_A4|CART_A5|CART_A6|CART_A7|CART_A8|CART_A9|CART_A10|CART_A11|CART_A12)

/* data bus pins */
#define CART_D0        (1ULL<<16)
#define CART_D1        (1ULL<<17)
#define CART_D2        (1ULL<<18)
#define CART_D3        (1ULL<<19)
#define CART_D4        (1ULL<<20)
#define CART_D5        (1ULL<<21)
#define CART_D6        (1ULL<<22)
#define CART_D7        (1ULL<<23)
#define CART_DB_PINS   (0xFF0000ULL)

#define CART_SET_DATA(p,d) {p=((p&~CART_DB_PINS)|((((uint64_t)d)&0xFF)<<16));}
#define CART_GET_DATA(p) ((uint8_t)(p>>16))
#define CART_SET_ADDR(p,a) {p=(p&~CART_AB_PINS)|((((uint64_t)a)&0x1FFFULL)<<0);}
#define CART_GET_ADDR(p) ((uint16_t)(p&CART_AB_PINS))
#define CART_GET_CS(p) ((p)&CART_CS)
#define CART_SET_CS(p) {p|=CART_CS;}
#define CART_RESET_CS(p) {p&=~CART_CS);}

/* tia state */
typedef struct {
    uint8_t rom[0x1000];
} cart_t;

void cart_init(cart_t* cart)
{
    memset(cart->rom, '\0', sizeof(cart->rom));
}

bool cart_load(cart_t* cart, uint8_t* content, uint16_t len, uint16_t offset)
{
    if(offset+len > 0x1000)
    {
        return false;
    }

    memcpy(&cart->rom[offset], content, len);

    return true;
}

uint64_t cart_read(cart_t* cart, uint64_t pins)
{
    if((pins & CART_A12) == CART_A12)
    {
        uint16_t addr = CART_GET_ADDR(pins) & (CART_A0|CART_A1|CART_A2|CART_A3|CART_A4|CART_A5|CART_A6|CART_A7|CART_A8|CART_A9|CART_A10|CART_A11);
        CART_SET_DATA(pins, cart->rom[addr]);
    }

	return pins;
}
