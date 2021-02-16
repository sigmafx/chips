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
#define TIA_A0        (1ULL<<0)
#define TIA_A1        (1ULL<<1)
#define TIA_A2        (1ULL<<2)
#define TIA_A3        (1ULL<<3)
#define TIA_A4        (1ULL<<4)
#define TIA_A5        (1ULL<<5)
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
#define TIA_RDY       (1ULL<<29)

/* input port pins */
#define TIA_I0        (1ULL<<48)
#define TIA_I1        (1ULL<<49)
#define TIA_I2        (1ULL<<50)
#define TIA_I3        (1ULL<<51)
#define TIA_I4        (1ULL<<52)
#define TIA_I5        (1ULL<<53)
#define TIA_PA_PINS   (TIA_I0|TIA_I1|TIA_I2|TIA_I3|TIA_I4|TIA_I5)

enum TV {
    NTSC = 0,
    PAL = 1
};

/* I/O port state */
typedef struct {
    uint8_t dr;
    uint8_t ddr;
} tia_port_t;

/* tia state */
typedef struct {
    enum TV tv;
    uint16_t scanclock;
	uint16_t scanline;
    uint8_t regWrite[0x2D];
    uint8_t regRead[0x0D];
} tia_t;

/* extract 8-bit data bus from 64-bit pins */
#define TIA_GET_DATA(p) ((uint8_t)(p>>16))
/* merge 8-bit data bus value into 64-bit pins */
#define TIA_SET_DATA(p,d) {p=((p&~0xFF0000)|((((uint64_t)d)&0xFF)<<16));}
/* extract port A pins */
#define TIA_GET_I(p) ((uint8_t)(p>>48))
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
#define TIA_GET_RDY(p) ((p)&TIA_RDY)

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
#define TIA_SET_RDY(p) (p|=TIA_RDY)
#define TIA_RESET_RDY(p) (p&=~TIA_RDY)

/* initialize a new 6532 instance */
uint64_t tia_init(tia_t* tia, enum TV tv);
/* tick the tia */
uint64_t tia_tick(tia_t* tia, uint64_t pins);

#ifdef __cplusplus
} /* extern "C" */
#endif

/*-- IMPLEMENTATION ----------------------------------------------------------*/
#define CHIPS_IMPL 
#ifdef CHIPS_IMPL
#include <string.h>
#ifndef CHIPS_ASSERT
#include <assert.h>
#define CHIPS_ASSERT(c) assert(c)
#endif

// Write registers
#define VSYNC   0x00
#define VBLANK  0x01
#define WSYNC   0x02
#define RSYNC   0x03
#define NUSIZ0  0x04
#define NUSIZ1  0x05
#define COLUP0  0x06
#define COLUP1  0x07
#define COLUPF  0x08
#define COLUBK  0x09
#define CTRLPF  0x0A
#define REFP0   0x0B
#define REFP1   0x0C
#define PF0     0x0D
#define PF1     0x0E
#define PF2     0x0F
#define RESP0   0x10
#define RESP1   0x11
#define RESM0   0x12
#define RESM1   0x13
#define RESBL   0x14
#define AUDC0   0x15
#define AUDC1   0x16
#define AUDF0   0x17
#define AUDF1   0x18
#define AUDV0   0x19
#define AUDV1   0x1A
#define GRP0    0x1B
#define GRP1    0x1C
#define ENAM0   0x1D
#define ENAM1   0x1E
#define ENABL   0x1F
#define HMP0    0x20
#define HMP1    0x21
#define HMM0    0x22
#define HMM1    0x23
#define HMBL    0x24
#define VDELP0  0x25
#define VDELP1  0x26
#define VDELBL  0x27
#define RESMP0  0x28
#define RESMP1  0x29
#define HMOVE   0x2A
#define HMCLR   0x2B
#define CXCLR   0x2C

// Read registers
#define CXM0P   0x00
#define CXM1P   0x01
#define CXP0FB  0x02
#define CXP1FB  0x03
#define CXM0FB  0x04
#define CXM1FB  0x05
#define CXBLPF  0x06
#define CXPPMM  0x07
#define INPT0   0x08
#define INPT1   0x09
#define INPT2   0x0A
#define INPT3   0x0B
#define INPT4   0x0C
#define INPT5   0x0D

uint64_t tia_init(tia_t* c, enum TV tv)
{
    c->tv = tv;
    c->scanclock = 0;
    c->scanline = 0;

    return 0ULL;
}

uint64_t tia_tick(tia_t* c, uint64_t pins)
{
    if ((pins & (TIA_CS0|TIA_CS1|TIA_CS2|TIA_CS3)) == TIA_CS1) {
        uint8_t addr = TIA_GET_ADDR(pins);
        uint8_t data = TIA_GET_DATA(pins);

        if(TIA_GET_RW(pins))
        {
            // Read
            switch (addr)
            {
                case WSYNC:
					c->regWrite[WSYNC] = 0xFF;
                    break;

                case RSYNC:
                    break;

                case RESP0:
                    break;

                case RESP1:
                    break;

                case RESM0:
                    break;

                case RESM1:
                    break;

                case RESBL:
                    break;

                default:
                    // Write to register
                    c->regWrite[addr] = data;
                    break;
            }
        }
        else
        {
            // Write
        }
    }

    c->scanclock++;
    if(c->scanclock == 228)
    {
        c->scanclock = 0;
		c->scanline++;

        // Release CPU
		c->regWrite[WSYNC] = 0x00;
    }

	if (c->regWrite[VSYNC] & 0b00000010)
	{
		c->scanline = 0;
	}

	if (c->scanclock < 68)
	{
		// Horizontal Blank / Sync
	}

    // Set RDY latch
    c->regWrite[WSYNC] ? TIA_SET_RDY(pins) : TIA_RESET_RDY(pins);

    return pins;
}
#endif // CHIPS_IMPL