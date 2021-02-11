#pragma once
/*#
    # m6532.h

    Header-only MOS 6532 Memory / IO / Timer Array emulator written in C.

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
    *   D0  <-->|           |<--> PA7   *
    *        ...|           |...        *
    *   D7  <-->|           |<--> PA0   *
    *           |           |           *
    *   A0  --->|           |<--> PB7   *
    *        ...|           |...        *
    *   A6  --->|   m6532   |<--> PB0   *
    *           |           |           *
    *   CS1 --->|           |<--- RS    *
    *   CS2 --->|           |           *
    *   RW  --->|           |---> IRQ   *
    *   RES --->|           |           *
    *           +-----------+           *
    *************************************

    ## How to use

    Call m6532_init() to initialize a new m6532_t instance (note that
    there is no m6532_desc_t struct:

    ~~~C
    m6532_t riot;
    m6532_init(&riot);
    ~~~

    In each system tick, call the m6532_tick() function, this takes
    an input pin mask, and returns a (potentially modified) output
    pin mask.

    Depending on the emulated system, the I/O pins PA0..PA7, PB0..PB7
    and the Interval Timer must be set as needed in the input pin mask
    (these are often connected to the keyboard matrix or peripheral devices).

    If the CPU wants to address the 128 byte RAM, set CS1/CS2 and RS RAM Select

    If the CPU wants to address the internal registers, set CS1/CS2 and
    deselct RS.

    On return m6532_tick() returns a modified pin mask where the following
    pins might have changed state:

    - the IRQ pin (same bit position as M6502_IRQ)
    - the port A I/O pins PA0..PA7
    - the port B I/O pins PB0..PB7
    - data bus pins D0..D7 if this was a register or ram read function.

    To reset a m6532_t instance, call m6532_reset():

    ## LINKS

    ## zlib/libpng license

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
#define M6532_A0       (1ULL<<0)
#define M6532_A1       (1ULL<<1)
#define M6532_A2       (1ULL<<2)
#define M6532_A3       (1ULL<<3)
#define M6532_A4       (1ULL<<4)
#define M6532_A5       (1ULL<<5)
#define M6532_A6       (1ULL<<6)
#define M6532_AB_PINS   (M6532_A0|M6532_A1|M6532_A2|M6532_A3|M6532_A4|M6532_A5|M6532_A6)

/* data bus pins */
#define M6532_D0        (1ULL<<16)
#define M6532_D1        (1ULL<<17)
#define M6532_D2        (1ULL<<18)
#define M6532_D3        (1ULL<<19)
#define M6532_D4        (1ULL<<20)
#define M6532_D5        (1ULL<<21)
#define M6532_D6        (1ULL<<22)
#define M6532_D7        (1ULL<<23)
#define M6532_DB_PINS   (0xFF0000ULL)

/* control pins */
#define M6532_CS1       (1ULL<<24)
#define M6532_CS2       (1ULL<<25)
#define M6532_RW        (1ULL<<26)
#define M6532_RS        (1ULL<<27)
#define M6532_IRQ       (1ULL<<28)

/* peripheral A port pins */
#define M6532_PA0       (1ULL<<48)
#define M6532_PA1       (1ULL<<49)
#define M6532_PA2       (1ULL<<50)
#define M6532_PA3       (1ULL<<51)
#define M6532_PA4       (1ULL<<52)
#define M6532_PA5       (1ULL<<53)
#define M6532_PA6       (1ULL<<54)
#define M6532_PA7       (1ULL<<55)
#define M6532_PA_PINS   (M6532_PA0|M6532_PA1|M6532_PA2|M6532_PA3|M6532_PA4|M6532_PA5|M6532_PA6|M6532_PA7)

/* peripheral B port pins */
#define M6532_PB0       (1ULL<<56)
#define M6532_PB1       (1ULL<<57)
#define M6532_PB2       (1ULL<<58)
#define M6532_PB3       (1ULL<<59)
#define M6532_PB4       (1ULL<<60)
#define M6532_PB5       (1ULL<<61)
#define M6532_PB6       (1ULL<<62)
#define M6532_PB7       (1ULL<<63)
#define M6532_PB_PINS   (M6532_PB0|M6532_PB1|M6532_PB2|M6532_PB3|M6532_PB4|M6532_PB5|M6532_PB6|M6532_PB7)

/* Interrupt flag bits */
#define M6532_INTFLAG_PA7   (1U<<6)
#define M6532_INTFLAG_TIMER (1U<<7)

/* I/O port state */
typedef struct {
    uint8_t dr;
    uint8_t ddr;
} m6532_port_t;

typedef struct {
    bool enabled;
    bool positive;
    bool porta7_last;
} m6532_edgedetect_t;

typedef struct {
    bool enabled;
    uint16_t ticks;
    uint8_t intervals;
    uint8_t divideby;
} m6532_timer_t;

/* m6532 state */
typedef struct {
    uint8_t ram[1<<7]; // Internal 128 bytes of ram
    m6532_port_t porta;
    m6532_port_t portb;
    m6532_timer_t timer;
    m6532_edgedetect_t edgedetect;
    uint8_t intflag;
} m6532_t;

/* extract 8-bit data bus from 64-bit pins */
#define M6532_GET_DATA(p) ((uint8_t)(p>>16))
/* merge 8-bit data bus value into 64-bit pins */
#define M6532_SET_DATA(p,d) {p=((p&~0xFF0000)|((((uint64_t)d)&0xFF)<<16));}
/* extract port A pins */
#define M6532_GET_PA(p) ((uint8_t)(p>>48))
/* extract port B pins */
#define M6532_GET_PB(p) ((uint8_t)(p>>56))
/* merge port A pins into pin mask */
#define M6532_SET_PA(p,a) {p=(p&0xFF00FFFFFFFFFFFFULL)|((((uint64_t)a)&0xFFULL)<<48);}
/* merge port B pins into pin mask */
#define M6532_SET_PB(p,b) {p=(p&0x00FFFFFFFFFFFFFFULL)|((((uint64_t)b)&0xFFULL)<<56);}
/* merge port A and B pins into pin mask */
#define M6532_SET_PAB(p,a,b) {p=(p&0x0000FFFFFFFFFFFFULL)|((((uint64_t)a)&0xFFULL)<<48)|((((uint64_t)b)&0xFFULL)<<56);}
/* extract 7-bit address from 64-bit pins */
#define M6532_GET_ADDR(p) ((uint8_t)(p&M6532_AB_PINS))
/* set 7-bit address to 64-bit pins */
#define M6532_SET_ADDR(p,a) {p=(p&~M6532_AB_PINS)|((((uint64_t)a)&0x7FULL)<<0);}

/* Get control pins */
#define M6532_GET_CS1(p) ((p)&M6532_CS1)
#define M6532_GET_CS2(p) ((p)&M6532_CS2)
#define M6532_GET_RW(p) ((p)&M6532_RW)
#define M6532_GET_RS(p) ((p)&M6532_RS)
#define M6532_GET_IRQ(p) ((p)&M6532_IRQ)
/* Set/reset input control pins */
#define M6532_SET_CS1(p) (p|=M6532_CS1)
#define M6532_RESET_CS1(p) (p&=~M6532_CS1)
#define M6532_SET_CS2(p) (p|=M6532_CS2)
#define M6532_RESET_CS2(p) (p&=~M6532_CS2)
#define M6532_SET_RW(p) (p|=M6532_RW)
#define M6532_RESET_RW(p) (p&=~M6532_RW)
#define M6532_SET_RS(p) (p|=M6532_RS)
#define M6532_RESET_RS(p) (p&=~M6532_RS)

/* initialize a new 6532 instance */
void m6532_init(m6532_t* m6532);
/* reset an existing 6532 instance */
void m6532_reset(m6532_t* m6532);
/* tick the m6532 */
uint64_t m6532_tick(m6532_t* m6532, uint64_t pins);

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

/* Set / reset output control bits */
#define M6532_SET_IRQ(p) (p&=~M6532_IRQ)
#define M6532_RESET_IRQ(p) (p|=M6532_IRQ)

static void _m6532_init_port(m6532_port_t* p) {
    p->dr = 0;
    p->ddr = 0;
}

static void _m6532_init_timer(m6532_timer_t* t) {
}

static void _m6532_init_edgedetect(m6532_edgedetect_t* e) {
}

void m6532_reset(m6532_t* c) {
    CHIPS_ASSERT(c);
    memset(c, 0, sizeof(*c));
    _m6532_init_port(&c->porta);
    _m6532_init_port(&c->portb);
    _m6532_init_timer(&c->timer);
    _m6532_init_edgedetect(&c->edgedetect);

    c->intflag = 0x00;
}

void m6532_init(m6532_t* c) {
    m6532_reset(c);
}

static inline uint8_t _m6532_read_ram(m6532_t* c, uint8_t addr) {
    return c->ram[addr];
}

static inline void _m6532_write_ram(m6532_t* c, uint8_t addr, uint8_t data) {
    c->ram[addr] = data;
}

static uint8_t _m6532_read_port(m6532_t* c, uint8_t addr, uint64_t pins) {
    uint8_t data = 0x00;

    switch(addr & (M6532_A0 | M6532_A1))
    {
        case 0b00:
            data = (c->porta.ddr & c->porta.dr) | (~c->porta.ddr & M6532_GET_PA(pins));
            break;

        case 0b01:
            data = c->porta.ddr;
            break;

        case 0b10:
            data = (c->portb.ddr & c->portb.dr) | (~c->portb.ddr & M6532_GET_PB(pins));
            break;

        case 0b11:
            data = c->portb.ddr;
            break;
    }

    return data;
}

static void _m6532_write_port(m6532_t* c, uint8_t addr, uint8_t data) {
    switch(addr & (M6532_A0 | M6532_A1))
    {
        case 0b00:
            c->porta.dr = data;
            break;

        case 0b01:
            c->porta.ddr = data;
            break;

        case 0b10:
            c->portb.dr = data;
            break;

        case 0b11:
            c->portb.ddr = data;
            break;
    }
}

static uint8_t _m6532_read_timerint(m6532_t* c, uint8_t addr, uint64_t pins) {
    uint8_t data;

    if(addr & M6532_A0) {
        // Timer
        data = c->timer.intervals;
        c->timer.enabled = (addr & M6532_A3);

        // Reset int flag timer bit
        c->intflag &= ~M6532_INTFLAG_TIMER;
    }
    else {
        data = c->intflag;

        // Reset int flag pa7 bit
        c->intflag &= ~M6532_INTFLAG_PA7;
    }

    return data;
}

static void _m6532_write_timerint(m6532_t* c, uint8_t addr, uint8_t data) {
    if(addr & M6532_A4) {
        // Timer
        c->timer.enabled = (addr & M6532_A3);
        c->timer.ticks = 0;
        c->timer.intervals = data;
        c->timer.divideby = (addr & (M6532_A0|M6532_A1));

        // Reset int flag timer bit
        c->intflag &= ~M6532_INTFLAG_TIMER;
    }
    else  {
        // Edge Detect Control - data is ignored
        c->edgedetect.enabled = (addr & M6532_A1);
        c->edgedetect.positive = (addr & M6532_A0);
    }
}

static void _m6532_timer_tick(m6532_t* c) {
    if(c->intflag & M6532_INTFLAG_TIMER) {
        // When interrupt triggered intervals become ticks
        c->timer.intervals--;
    }
    else {
        // Ticks by divder
        if(c->timer.divideby & c->timer.ticks) {
            c->timer.ticks = 0;
            c->timer.intervals--;
        }
        else {
            c->timer.ticks++;
        }
    }
}

uint64_t m6532_tick(m6532_t* c, uint64_t pins) {
    // Count ticks even if chip not enabled
    if(c->timer.enabled) {
        _m6532_timer_tick(c);
    }

    if ((pins & (M6532_CS1|M6532_CS2)) == M6532_CS1) {
        uint8_t addr = M6532_GET_ADDR(pins);
        uint8_t data = M6532_GET_DATA(pins);

        if(M6532_GET_RS(pins)) {
            if(addr & M6532_A2) {
                // Timer / Interrupt
                if(M6532_GET_RW(pins)) {
                    // Read
                    M6532_SET_DATA(pins, _m6532_read_timerint(c, addr, pins));
                }
                else {
                    // Write
                    _m6532_write_timerint(c, addr, data);
                }
            }
            else {
                // Ports
                if(M6532_GET_RW(pins)) {
                    // Read
                    M6532_SET_DATA(pins, _m6532_read_port(c, addr, pins));
                }
                else {
                    // Write
                    _m6532_write_port(c, addr, data);
                }
            }
        }
        else {
            // Memory
            if(M6532_GET_RW(pins)) {
                // Read
                M6532_SET_DATA(pins, _m6532_read_ram(c, addr));
            }
            else {
                // Write
                _m6532_write_ram(c, addr, data);
            }
        }
    }

    // Write port outputs
    uint8_t port;
    // Port A
    port = M6532_GET_PA(pins);
    port &= ~c->porta.ddr;
    port |= (c->porta.dr & c->porta.ddr);
    M6532_SET_PA(pins, port);
    // Port B
    port=M6532_GET_PB(pins);
    port &= ~c->portb.ddr;
    port |= (c->portb.dr & c->portb.ddr);
    M6532_SET_PB(pins, port);

    // Edge Detect
    // Input and output DDR can trigger the IRQ
    if(c->edgedetect.enabled) {
        if((c->edgedetect.positive && !c->edgedetect.porta7_last && (pins & M6532_PA7) || (!c->edgedetect.positive && c->edgedetect.porta7_last && !(pins & M6532_PA7)))) {
            c->intflag |= M6532_INTFLAG_PA7;
        }
    }
    c->edgedetect.porta7_last = pins & M6532_PA7;

    // Timer
    if(c->timer.enabled) {
        if(c->timer.intervals == 0) {
            c->intflag |= M6532_INTFLAG_TIMER;
        }
    }

    // Set interrupt
    if(c->intflag) {
        M6532_RESET_IRQ(pins);
    }
    else {
        M6532_SET_IRQ(pins);
    }

    return pins;
}
#endif // CHIPS_IMPL