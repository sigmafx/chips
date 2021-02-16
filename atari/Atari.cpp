#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdint.h>
#include <stdio.h>

#include "Atari.h"
#define CHIPS_IMPL
#include "../chips/m6502.h"
#include "../chips/m6532.h"
#include "../chips/cartridge.h"
#include "../chips/tia.h"

COLORREF screen[228][262];
HWND hDlgMain;
int x, y;

cart_t cart;
m6502_t m6502;
m6532_t m6532;
tia_t tia;

void tick(uint8_t red, uint8_t green, uint8_t blue)
{
	screen[x][y] = RGB(red, green, blue);
	x++;
}

void OnHSync(bool active)
{
	y++;
	x = 0;
}

void OnVSync(bool active)
{
}

void OnVBlank(bool active)
{
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE /* hPrevInstance */,
                     _In_ LPWSTR    /* lpCmdLine */,
                     _In_ int       /* nCmdShow */)
{
	m6502_desc_t m6502_desc;
	uint64_t pins_6502;
	uint64_t pins_cart;
	uint64_t pins_tia;
	uint64_t pins_6532;
	uint8_t clock = 0;

    memset(screen, 0x7F, sizeof(DWORD) * 228 * 262);
	x = y = 0;

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
	pins_tia = tia_init(&tia, TV::NTSC);

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

		if (clock == 0)
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
			pins_6502 & M6502_A12 ? M6532_RESET_CS2(pins_6532) : M6532_SET_CS2(pins_6532);
			pins_6502 & M6502_A9 ? M6532_SET_RS(pins_6532) : M6532_RESET_RS(pins_6532);
			pins_6502 & M6502_RW ? M6532_SET_RW(pins_6532) : M6532_RESET_RW(pins_6532);
			// TIA
			pins_6502 & M6502_A12 ? TIA_RESET_CS0(pins_tia) : TIA_SET_CS0(pins_tia);
			TIA_SET_CS1(pins_tia);
			pins_6502 & M6502_A7 ? TIA_RESET_CS1(pins_tia) : TIA_SET_CS1(pins_tia);
			pins_6502 & M6502_RW ? TIA_SET_RW(pins_tia) : TIA_RESET_RW(pins_tia);

			if (!(pins_6502 & M6502_RW))
			{
				// Write
				// Assert the data lines of all chips
				CART_SET_DATA(pins_6532, data);
				M6532_SET_DATA(pins_6532, data);
				TIA_SET_DATA(pins_tia, data);
			}

			pins_cart = cart_read(&cart, pins_cart);
			pins_6532 = m6532_tick(&m6532, pins_6532);
		}

		pins_tia = tia_tick(&tia, pins_tia);

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

		// Set RDY
		TIA_GET_RDY(pins_tia) ? M6502_SET_RDY(pins_6502) : M6502_RESET_RDY(pins_6502);

		clock++;
		if (clock == 3)
		{
			clock = 0;
		}
	}

    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DLG_MAIN), HWND_DESKTOP, DlgProc);
}

void OnPaint(HWND hDlg)
{
    HDC hdc = GetDC(hDlg);
    HDC hdc2 = CreateCompatibleDC(hdc);
    HBITMAP hbmp;
    HGDIOBJ hold;
    int x, y;

    hbmp = CreateCompatibleBitmap(hdc, 228, 262);
    hold = SelectObject(hdc2, hbmp);

    for (x = 0; x < 228; x++)
    {
        for (y = 0; y < 262; y++)
        {
            SetPixel(hdc2, x, y, screen[x][y]);
        }
    }

    BitBlt(hdc, 0, 0, 228, 262, hdc2, 0, 0, SRCCOPY);

    DeleteObject(SelectObject(hdc2, hold));
    DeleteDC(hdc2);
    ReleaseDC(hDlg, hdc);
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret;

    switch (msg)
    {
    case WM_INITDIALOG:
    {
        hDlgMain = hDlg;
        SetWindowText(hDlg, _T("Atari"));
        SetWindowPos(hDlg, NULL, 0, 0, 228 + 16, 262 + 38, SWP_NOMOVE | SWP_NOZORDER);
        ret = TRUE;
        break;
    }

    case WM_PAINT:
        OnPaint(hDlg);
        ret = TRUE;
        break;

    case WM_ERASEBKGND:
        ret = TRUE;
        break;

    case WM_CLOSE:
        EndDialog(hDlg, FALSE);
        ret = TRUE;
        break;

    default:
        ret = FALSE;
            break;
    }

    return ret;
}
