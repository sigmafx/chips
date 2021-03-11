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
    *   A5  --->|    TIA    |           *
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

// Stella Programmer's Guide PDF
// https://www.atarihq.com/danb/files/stella.pdf
// https://www.atarihq.com/danb/files/

// Interesting notes about TIA and timings
// http://atarihq.com/danb/files/TIA_HW_Notes.txt

// TIA Pinout
// https://www.atarihq.com/danb/tia.shtml

// 2600 Programming
// https://www.randomterrain.com/atari-2600-memories.html#assembly_language

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

enum Direction {
    forward = 0,
    reverse = 1
};

typedef void(*cb_tv)(uint8_t);

/* I/O port state */
typedef struct {
    uint8_t dr;
    uint8_t ddr;
} tia_port_t;

/* Bit register */
typedef struct {
    bool enabled;
    uint8_t width;

    uint8_t ticks;
    uint8_t reg;
    uint8_t delayreg;
} tia_bitreg_t;

/* Shift register */
typedef struct {
    uint8_t width;

    bool enabled;
    enum Direction direction;
    uint8_t divider;

    uint8_t ticks;
    uint8_t bits;
    uint8_t reg;
    uint8_t delayreg;
} tia_shiftreg_t;

/* TIA state */
typedef struct {
    enum TV tv;
    uint16_t colourclock;
    uint8_t regWrite[0x2D];
    uint8_t regRead[0x0E];
    cb_tv colourtick;
    cb_tv hsync;
    cb_tv vsync;

    uint8_t posBL;
    uint8_t posM0;
    uint8_t posM1;
    uint8_t posP0;
    uint8_t posP1;

    tia_bitreg_t bitBL;
    tia_bitreg_t bitM0;
    tia_bitreg_t bitM1;
    tia_shiftreg_t shiftPF0;
    tia_shiftreg_t shiftPF1;
    tia_shiftreg_t shiftPF2;
    tia_shiftreg_t shiftP0;
    tia_shiftreg_t shiftP1;
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

/* initialize a new TIA instance */
uint64_t tia_init(tia_t* tia, enum TV tv, cb_tv colourtick, cb_tv hsync, cb_tv vsync);
/* tick the TIA */
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

// Register bit masks
#define TIA_BIT_D0              0x01
#define TIA_BIT_D1              0x02
#define TIA_BIT_D2              0x04
#define TIA_BIT_D3              0x08
#define TIA_BIT_D4              0x10
#define TIA_BIT_D5              0x20
#define TIA_BIT_D6              0x40
#define TIA_BIT_D7              0x80

// Object visibility for current colour clock
#define TIA_OBJECT_BIT_PF       0b00000001
#define TIA_OBJECT_BIT_P0       0b00000010
#define TIA_OBJECT_BIT_P1       0b00000100
#define TIA_OBJECT_BIT_M0       0b00001000
#define TIA_OBJECT_BIT_M1       0b00010000
#define TIA_OBJECT_BIT_BL       0b00100000

// Bit register
void tia_bitreg_init(tia_bitreg_t* bitreg)
{
    bitreg->enabled = false;
    bitreg->delayreg = 0x00;
}

void tia_bitreg_start(tia_bitreg_t* bitreg, uint8_t divider, uint8_t set, uint8_t ffwd)
{
    bitreg->width = 1 << divider;

    if (ffwd < bitreg->width)
    {
        bitreg->enabled = true;
        bitreg->reg = set;
        bitreg->ticks = ffwd;
    }
    else
    {
        bitreg->enabled = false;
    }
}

bool tia_bitreg_tick(tia_bitreg_t* bitreg, bool delay)
{
    bool ret = false;

    if (bitreg->enabled)
    {
        ret = (delay ? bitreg->delayreg : bitreg->reg) & 0x01;

        bitreg->ticks++;

        if (bitreg->ticks == bitreg->width)
        {
            bitreg->delayreg = bitreg->reg;
            bitreg->enabled = false;
        }
    }

    return ret;
}

// Shift register
void tia_shiftreg_init(tia_shiftreg_t* shiftreg, uint8_t width)
{
    shiftreg->width = width;
    shiftreg->enabled = false;
    shiftreg->delayreg = 0x00;
}

void tia_shiftreg_enable(tia_shiftreg_t* shiftreg, uint8_t divider, enum Direction direction, uint8_t set)
{
    shiftreg->enabled = true;
    shiftreg->direction = direction;
    shiftreg->divider = divider;
    shiftreg->ticks = 0;
    shiftreg->bits = 0;
    shiftreg->reg = set;
}

bool tia_shiftreg_tick(tia_shiftreg_t* shiftreg, bool delay)
{
    bool ret = false;

    if (shiftreg->enabled)
    {
        uint8_t maskIn = 1 << (shiftreg->direction == reverse ? 0x00 : shiftreg->width - 1);
        uint8_t maskOut = 1 << (shiftreg->direction == reverse ? shiftreg->width - 1 : 0x00);

        ret = (delay ? shiftreg->delayreg : shiftreg->reg) & maskOut;

        shiftreg->ticks++;

        if (shiftreg->bits != (shiftreg->ticks / shiftreg->divider))
        {
            // Shift
            shiftreg->delayreg = shiftreg->direction == reverse ? shiftreg->delayreg << 1 : shiftreg->delayreg >> 1;
            shiftreg->delayreg |= shiftreg->reg & maskOut ? maskIn : 0x00;
            shiftreg->reg = shiftreg->direction == reverse ? shiftreg->reg << 1 : shiftreg->reg >> 1;
            shiftreg->bits++;

            if (shiftreg->bits == shiftreg->width)
            {
                shiftreg->enabled = false;
            }
        }
    }

    return ret;
}

uint64_t tia_init(tia_t* c, enum TV tv, cb_tv colourtick, cb_tv hsync, cb_tv vsync)
{
    memset(c, '\0', sizeof(tia_t));

    c->tv = tv;

	c->colourtick = colourtick;
	c->hsync = hsync;
	c->vsync = vsync;

    tia_bitreg_init(&c->bitBL);
    tia_bitreg_init(&c->bitM0);
    tia_bitreg_init(&c->bitM1);
    tia_shiftreg_init(&c->shiftPF0, 4);
    tia_shiftreg_init(&c->shiftPF1, 8);
    tia_shiftreg_init(&c->shiftPF2, 8);
    tia_shiftreg_init(&c->shiftP0, 8);
    tia_shiftreg_init(&c->shiftP1, 8);

    return 0ULL;
}

uint64_t tia_tick(tia_t* c, uint64_t pins)
{
    int16_t pixelclock = c->colourclock - 68;

    if ((pins & (TIA_CS0|TIA_CS1|TIA_CS2|TIA_CS3)) == TIA_CS1) {
        uint8_t addr = TIA_GET_ADDR(pins);

        if(TIA_GET_RW(pins))
        {
            // Read
            TIA_SET_DATA(pins, c->regRead[addr]);
        }
        else
        {
            uint8_t data = TIA_GET_DATA(pins);

            // Write
			switch (addr)
			{
			case WSYNC:
				c->regWrite[WSYNC] = 0x01;
				break;

			case RSYNC:
				c->regWrite[RSYNC] = 0x01;
				break;

			case RESP0:
                c->posP0 = (pixelclock < 0) ? 0 : pixelclock;
                break;

			case RESP1:
                c->posP1 = (pixelclock < 0) ? 0 : pixelclock;
                break;

			case RESM0:
                c->posM0 = (pixelclock < 0) ? 0 : pixelclock;
                break;

			case RESM1:
                c->posM1 = (pixelclock < 0) ? 0 : pixelclock;
                break;

			case RESBL:
                c->posBL = (pixelclock < 0) ? 0 : pixelclock;
				break;

			default:
				// Write to register
				c->regWrite[addr] = data;
				break;
			}
		}
    }

    if (pixelclock < 0)
    {
        // Horizontal blanking
        if (c->colourtick != NULL)
        {
            c->colourtick(0);
        }
    }
    else
    {
        uint8_t pixels = 0x00;
        uint8_t regColu;

        // PF - Playfield
        switch (pixelclock)
        {
        case 0:
            tia_shiftreg_enable(&c->shiftPF0, 4, forward, c->regWrite[PF0] >> 4);
            break;

        case 16:
            tia_shiftreg_enable(&c->shiftPF1, 4, reverse, c->regWrite[PF1]);
            break;

        case 48:
            tia_shiftreg_enable(&c->shiftPF2, 4, forward, c->regWrite[PF2]);
            break;

        case 80:
            if (!(c->regWrite[CTRLPF] & TIA_BIT_D0))
            {
                tia_shiftreg_enable(&c->shiftPF0, 4, forward, c->regWrite[PF0] >> 4);
            }
            else
            {
                tia_shiftreg_enable(&c->shiftPF2, 4, reverse, c->regWrite[PF2]);
            }
            break;

        case 96:
            if (!(c->regWrite[CTRLPF] & TIA_BIT_D0))
            {
                tia_shiftreg_enable(&c->shiftPF1, 4, reverse, c->regWrite[PF1]);
            }
            break;

        case 112:
            if (c->regWrite[CTRLPF] & TIA_BIT_D0)
            {
                tia_shiftreg_enable(&c->shiftPF1, 4, forward, c->regWrite[PF1]);
            }
            break;

        case 128:
            if (!(c->regWrite[CTRLPF] & TIA_BIT_D0))
            {
                tia_shiftreg_enable(&c->shiftPF2, 4, forward, c->regWrite[PF2]);
            }
            break;

        case 144:
            if (c->regWrite[CTRLPF] & TIA_BIT_D0)
            {
                tia_shiftreg_enable(&c->shiftPF0, 4, reverse, c->regWrite[PF0] >> 4);
            }
            break;
        }

        // BL - Ball
        if (pixelclock == 0 || pixelclock == c->posBL)
        {
            tia_bitreg_start(&c->bitBL, (c->regWrite[CTRLPF] & (TIA_BIT_D4|TIA_BIT_D5)) >> 4, (c->regWrite[ENABL] & TIA_BIT_D1) >> 1, pixelclock == c->posBL ? 0 : 160 - c->posBL);
        }

        // P0 - Player 0
        if (pixelclock == c->posP0)// TODO Add copies
        {
            uint8_t size;

            size = (c->regWrite[NUSIZ0] & (TIA_BIT_D0|TIA_BIT_D1|TIA_BIT_D2));
            size = size == 0b101 ? 1 : size == 0b111 ? 2 : 0;

            tia_shiftreg_enable(&c->shiftP0, 1 << size, c->regWrite[REFP0] & TIA_BIT_D3 ? reverse : forward, c->regWrite[GRP0]);
        }

        // P1 - Player 1
        if (pixelclock == c->posP1)// TODO Add copies
        {
            uint8_t size;

            size = (c->regWrite[NUSIZ1] & (TIA_BIT_D0|TIA_BIT_D1|TIA_BIT_D2));
            size = size == 0b101 ? 1 : size == 0b111 ? 2 : 0;

            tia_shiftreg_enable(&c->shiftP1, 1 << size, c->regWrite[REFP1] & TIA_BIT_D3 ? reverse : forward, c->regWrite[GRP1]);
        }

        // M0 - Missile 0
        if (pixelclock == 0 || pixelclock == c->posM0)// TODO Add copies
        {
            tia_bitreg_start(&c->bitM0, (c->regWrite[NUSIZ0] & (TIA_BIT_D4|TIA_BIT_D5)) >> 4, c->regWrite[ENAM0] >> 1, pixelclock == c->posM0 ? 0 : 160 - c->posM0);
        }

        // M1 - Missile 1
        if (pixelclock == 0 || pixelclock == c->posM1)// TODO Add copies
        {
            tia_bitreg_start(&c->bitM1, (c->regWrite[NUSIZ1] & (TIA_BIT_D4|TIA_BIT_D5)) >> 4, c->regWrite[ENAM1] >> 1, pixelclock == c->posM1 ? 0 : 160 - c->posM1);
        }

        pixels |= tia_bitreg_tick(&c->bitBL, c->regWrite[VDELBL] & TIA_BIT_D0) ? TIA_OBJECT_BIT_BL : 0x00;
        pixels |= tia_shiftreg_tick(&c->shiftPF0, false) ? TIA_OBJECT_BIT_PF : 0x00;
        pixels |= tia_shiftreg_tick(&c->shiftPF1, false) ? TIA_OBJECT_BIT_PF : 0x00;
        pixels |= tia_shiftreg_tick(&c->shiftPF2, false) ? TIA_OBJECT_BIT_PF : 0x00;
        pixels |= tia_bitreg_tick(&c->bitM0, false) ? TIA_OBJECT_BIT_M0 : 0x00;
        pixels |= tia_bitreg_tick(&c->bitM1, false) ? TIA_OBJECT_BIT_M1 : 0x00;
        pixels |= tia_shiftreg_tick(&c->shiftP0, c->regWrite[VDELP0] & TIA_BIT_D0) ? TIA_OBJECT_BIT_P0 : 0x00;
        pixels |= tia_shiftreg_tick(&c->shiftP1, c->regWrite[VDELP1] & TIA_BIT_D0) ? TIA_OBJECT_BIT_P1 : 0x00;

        // Apply priorities / settings to decide colour
        if (pixels & TIA_OBJECT_BIT_BL)
        {
            regColu = COLUPF;
        }
        else
        if (pixels & TIA_OBJECT_BIT_PF)
        {
            if (c->regWrite[CTRLPF] & TIA_BIT_D1)
            {
                if (pixelclock < 80)
                {
                    regColu = COLUP0;
                }
                else
                {
                    regColu = COLUP1;
                }
            }
            else
            {
                regColu = COLUPF;
            }
        }
        else
        {
            regColu = COLUBK;
        }

        // Drawing pixel
        if (c->colourtick != NULL)
        {
            c->colourtick(c->regWrite[regColu]);
        }
    }

    // Horizontal sync
    if (c->colourclock == 227)
    {
        c->regWrite[WSYNC] = 0x00;

        if (c->hsync != NULL)
        {
            c->hsync(1);
        }
    }
    else
    {
        if (c->hsync != NULL)
        {
            c->hsync(0);
        }
    }

    // Vertical sync
    if (c->vsync != NULL)
    {
        if (c->regWrite[VSYNC] & 0b00000010)
        {
            c->vsync(1);
        }
        else
        {
            c->vsync(0);
        }
    }

    // Advance colour clock
    if (c->colourclock == 227)
    {
        c->colourclock = 0;
    }
    else
    {
        c->colourclock++;
    }

    // Set RDY
    c->regWrite[WSYNC] ? TIA_SET_RDY(pins) : TIA_RESET_RDY(pins);

    return pins;
}
#endif // CHIPS_IMPL