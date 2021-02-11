#pragma once
/*#
    # tia.h

    Header-only Atari TIA 1A emulator written in C.

    Do this:
    ~~~C
    #define CHIPS_IMPL
    ~~~
    before you include this file in *one* C or C++ file to create the
    implementation.

    Optionally provide the following macros with your own implementation

    ~~~C
    CHIPS_ASSERT(c)
    ~~~
        your own assert macro (default: assert(c))

    ## Emulated Pins
    *************************************
    *           +-----------+           *
    *   D0  <-->|           |           *
    *        ...|           |           *
    *   D7  <-->|           |           *
    *           |           |           *
    *   A0  --->|           |---> RDY   *
    *        ...|           |           *
    *   A5  --->|    tia    |           *
    *           |           |<--- RW    *
    *   I0  --->|           |<--- CS0   *
    *        ...|           |<--- CS1   *
    *   I5  --->|           |<--- CS2   *
    *           |           |<--- CS3   *
    *           +-----------+           *
    *************************************

    ## How to use
    Copyright (c) 2018 Andre Weissflog
    Copyright (c) 2021 David Robinson

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.
    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:
        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.
        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.
        3. This notice may not be removed or altered from any source
        distribution.
#*/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* address bus pins */
#define TIA_A0       (1ULL<<0)
#define TIA_A1       (1ULL<<1)
#define TIA_A2       (1ULL<<2)
#define TIA_A3       (1ULL<<3)
#define TIA_A4       (1ULL<<4)
#define TIA_A5       (1ULL<<5)
#define TIA_AB_PINS   (TIA_A0|TIA_A1|TIA_A2|TIA_A3|TIA_A4|TIA_A5)

/* data bus pins */
#define TIA_D0        (1ULL<<16)
#define TIA_D1        (1ULL<<17)
#define TIA_D2        (1ULL<<18)
#define TIA_D3        (1ULL<<19)
#define TIA_D4        (1ULL<<20)
#define TIA_D5        (1ULL<<21)
#define TIA_D6        (1ULL<<22)
#define TIA_D7        (1ULL<<23)
#define TIA_DB_PINS   (0xFF0000ULL)

/* control pins */
#define TIA_CS0       (1ULL<<24)
#define TIA_CS1       (1ULL<<25)
#define TIA_CS2       (1ULL<<26)
#define TIA_CS3       (1ULL<<27)
#define TIA_RW        (1ULL<<28)

/* input port pins */
#define TIA_I0       (1ULL<<48)
#define TIA_I1       (1ULL<<49)
#define TIA_I2       (1ULL<<50)
#define TIA_I3       (1ULL<<51)
#define TIA_I4       (1ULL<<52)
#define TIA_I5       (1ULL<<53)
#define TIA_PA_PINS   (TIA_I0|TIA_I1|TIA_I2|TIA_I3|TIA_I4|TIA_I5)

/* I/O port state */
typedef struct {
    uint8_t dr;
    uint8_t ddr;
} tia_port_t;

/* tia state */
typedef struct {
} tia_t;

/* extract 8-bit data bus from 64-bit pins */
#define TIA_GET_DATA(p) ((uint8_t)(p>>16))
/* merge 8-bit data bus value into 64-bit pins */
#define TIA_SET_DATA(p,d) {p=((p&~0xFF0000)|((((uint64_t)d)&0xFF)<<16));}
/* extract port A pins */
#define TIA_GET_I(p) ((uint8_t)(p>>48))
/* merge port A pins into pin mask */
#define TIA_SET_PA(p,a) {p=(p&0xFF00FFFFFFFFFFFFULL)|((((uint64_t)a)&0xFFULL)<<48);}
/* extract 6-bit address from 64-bit pins */
#define TIA_GET_ADDR(p) ((uint8_t)(p&TIA_AB_PINS))
/* set 6-bit address to 64-bit pins */
#define TIA_SET_ADDR(p,a) {p=(p&~TIA_AB_PINS)|((((uint64_t)a)&0x3FULL)<<0);}

/* Get control pins */
#define TIA_GET_CS0(p) ((p)&TIA_CS0)
#define TIA_GET_CS1(p) ((p)&TIA_CS1)
#define TIA_GET_CS2(p) ((p)&TIA_CS2)
#define TIA_GET_CS3(p) ((p)&TIA_CS3)
#define TIA_GET_RW(p) ((p)&TIA_RW)

/* Set/reset input control pins */
#define TIA_SET_CS0(p) (p|=TIA_CS0)
#define TIA_RESET_CS0(p) (p&=~TIA_CS0)
#define TIA_SET_CS1(p) (p|=TIA_CS1)
#define TIA_RESET_CS1(p) (p&=~TIA_CS1)
#define TIA_SET_CS2(p) (p|=TIA_CS2)
#define TIA_RESET_CS2(p) (p&=~TIA_CS2)
#define TIA_SET_CS3(p) (p|=TIA_CS3)
#define TIA_RESET_CS3(p) (p&=~TIA_CS3)
#define TIA_SET_RW(p) (p|=TIA_RW)
#define TIA_RESET_RW(p) (p&=~TIA_RW)

/* initialize a new 6532 instance */
void tia_init(tia_t* tia);
/* tick the tia */
uint64_t tia_tick(tia_t* tia, uint64_t pins);

#ifdef __cplusplus
} /* extern "C" */
#endif

/*-- IMPLEMENTATION ----------------------------------------------------------*/
#ifdef CHIPS_IMPL
#include <string.h>
#ifndef CHIPS_ASSERT
#include <assert.h>
#define CHIPS_ASSERT(c) assert(c)
#endif

uint64_t tia_tick(tia_t* c, uint64_t pins) {
    if ((pins & (TIA_CS0|TIA_CS1|TIA_CS2|TIA_CS3)) == TIA_CS1) {
        uint8_t addr = TIA_GET_ADDR(pins);
        uint8_t data = TIA_GET_DATA(pins);
    }

    return pins;
}
#endif // CHIPS_IMPL