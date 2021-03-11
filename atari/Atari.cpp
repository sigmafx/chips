#include <stdio.h>
#include "Atari.h"
#include "DlgComposite.h"

#define CHIPS_IMPL
#include "../chips/m6502.h"
#include "../chips/m6532.h"
#include "../chips/cartridge.h"
#include "../chips/tia.h"

cart_t cart;
m6502_t m6502;
m6532_t m6532;
tia_t tia;

DWORD WINAPI Start(LPVOID lpParam)
{
	m6502_desc_t m6502_desc;
	uint64_t pins_6502;
	uint64_t pins_cart;
	uint64_t pins_tia;
	uint64_t pins_6532;

	// Initialise CPU
	m6502_desc.bcd_disabled = true;
	m6502_desc.m6510_in_cb = NULL;
	m6502_desc.m6510_out_cb = NULL;
	m6502_desc.m6510_user_data = NULL;
	m6502_desc.m6510_io_pullup = 0;
	m6502_desc.m6510_io_floating = 0;
	pins_6502 = m6502_init(&m6502, &m6502_desc);

	// Initialise RIOT
	pins_6532 = m6532_init(&m6532);

	// Initialsie TIA
	pins_tia = tia_init(&tia, TV::NTSC, OnTick, OnHSync, OnVSync);

	// Initialise the cartridge
	pins_cart = cart_init(&cart);
	FILE* file;
	uint8_t content[0x1000];
	fopen_s(&file, "C:\\Users\\David\\Documents\\GitHub\\chips\\carts\\kernel_01.bin", "rb");
	fread(content, 0x1000, 1, file);
	fclose(file);
	cart_load(&cart, content, 0x1000, 0);

	while (true)
	{
		uint16_t addr = 0;
		uint8_t data = 0;

		Sleep(0);

		pins_tia = tia_tick(&tia, pins_tia);

		// Set RDY
		TIA_GET_RDY(pins_tia) ? M6502_SET_RDY(pins_6502) : M6502_RESET_RDY(pins_6502);

		if (TIA_GET_PH1(pins_tia))
		{
			pins_6502 = m6502_tick(&m6502, pins_6502);
			addr = M6502_GET_ADDR(pins_6502);

			// Assert address lines of all chips
			CART_SET_ADDR(pins_cart, addr);
			M6532_SET_ADDR(pins_6532, addr);
			TIA_SET_ADDR(pins_tia, addr);

			// Set control lines of all chips
			// Cart
			pins_6502 & M6502_A12 ? CART_SET_CS(pins_cart) : CART_RESET_CS(pins_cart);
			// 6532
			pins_6502 & M6502_A7 ? M6532_SET_CS1(pins_6532) : M6532_RESET_CS1(pins_6532);
			pins_6502 & M6502_A12 ? M6532_SET_CS2(pins_6532) : M6532_RESET_CS2(pins_6532);
			pins_6502 & M6502_A9 ? M6532_SET_RS(pins_6532) : M6532_RESET_RS(pins_6532);
			pins_6502 & M6502_RW ? M6532_SET_RW(pins_6532) : M6532_RESET_RW(pins_6532);
			// TIA
			pins_6502 & M6502_A12 ? TIA_SET_CS0(pins_tia) : TIA_RESET_CS0(pins_tia);
			TIA_SET_CS1(pins_tia);
			TIA_RESET_CS2(pins_tia);
			pins_6502 & M6502_A7 ? TIA_SET_CS3(pins_tia) : TIA_RESET_CS3(pins_tia);
			pins_6502 & M6502_RW ? TIA_SET_RW(pins_tia) : TIA_RESET_RW(pins_tia);

			if (!(pins_6502 & M6502_RW))
			{
				// Write
				data = M6502_GET_DATA(pins_6502);
				// Assert the data lines of all chips
				CART_SET_DATA(pins_6532, data);
				M6532_SET_DATA(pins_6532, data);
				TIA_SET_DATA(pins_tia, data);
			}

			pins_cart = cart_tick(&cart, pins_cart);
			pins_6532 = m6532_tick(&m6532, pins_6532);

			if (pins_6502 & M6502_RW)
			{
				// Read - determine enabled chip
				if (pins_6502 & M6502_A12)
				{
					// Cart enabled
					data = CART_GET_DATA(pins_cart);
				}
				else
				{
					if (pins_6502 & M6502_A7)
					{
						// 6532 enabled
						data = M6532_GET_DATA(pins_6532);
					}
					else
					{
						// TIA enabled
						data = TIA_GET_DATA(pins_tia);
					}
				}

				// Read
				// Assert the data lines of all chips
				M6502_SET_DATA(pins_6502, data);
				CART_SET_DATA(pins_cart, data);
				M6532_SET_DATA(pins_6532, data);
				TIA_SET_DATA(pins_tia, data);
			}
		}
	}
}
