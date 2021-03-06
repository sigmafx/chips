#include <tchar.h>

#include "DlgComposite.h"
#include "resource.h"

uint32_t ntscRGB[] = {
	0x000000,		// 00
	0x404040,		// 02
	0x6c6c6c,		// 04
	0x909090,		// 06
	0xb0b0b0,		// 08
	0xc8c8c8,		// 0A
	0xdcdcdc,		// 0C
	0xf4f4f4,		// 0E
	0x004444,		// 10
	0x106464,		// 12
	0x248484,		// 14
	0x34a0a0,		// 16
	0x40b8b8,		// 18
	0x50d0d0,		// 1A
	0x5ce8e8,		// 1C
	0x68fcfc,		// 1E
	0x002870,		// 20
	0x144484,		// 22
	0x285c98,		// 24
	0x3c78ac,		// 26
	0x4c8cbc,		// 28
	0x5ca0cc,		// 2A
	0x68b4dc,		// 2C
	0x78c8ec,		// 2E
	0x001884,		// 30
	0x183498,		// 32
	0x3050ac,		// 34
	0x4868c0,		// 36
	0x5c80d0,		// 38
	0x7094e0,		// 3A
	0x80a8ec,		// 3C
	0x94bcfc,		// 3E
	0x000088,		// 40
	0x20209c,		// 42
	0x3c3cb0,		// 44
	0x5858c0,		// 46
	0x7070d0,		// 48
	0x8888e0,		// 4A
	0xa0a0ec,		// 4C
	0xb4b4fc,		// 4E
	0x5c0078,		// 50
	0x74208c,		// 52
	0x883ca0,		// 54
	0x9c58b0,		// 56
	0xb070c0,		// 58
	0xc084d0,		// 5A
	0xd09cdc,		// 5C
	0xe0b0ec,		// 5E
	0x780048,		// 60
	0x902060,		// 62
	0xa43c78,		// 64
	0xb8588c,		// 66
	0xcc70a0,		// 68
	0xdc84b4,		// 6A
	0xec9cc4,		// 6C
	0xfcb0d4,		// 6E
	0x840014,		// 70
	0x982030,		// 72
	0xac3c4c,		// 74
	0xc05868,		// 76
	0xd0707c,		// 78
	0xe08894,		// 7A
	0xeca0a8,		// 7C
	0xfcb4bc,		// 7E
	0x880000,		// 80
	0x9c201c,		// 82
	0xb04038,		// 84
	0xc05c50,		// 86
	0xd07468,		// 88
	0xe08c7c,		// 8A
	0xeca490,		// 8C
	0xfcb8a4,		// 8E
	0x7c1800,		// 90
	0x90381c,		// 92
	0xa85438,		// 94
	0xbc7050,		// 96
	0xcc8868,		// 98
	0xdc9c7c,		// 9A
	0xecb490,		// 9C
	0xfcc8a4,		// 9E
	0x5c2c00,		// A0
	0x784c1c,		// A2
	0x906838,		// A4
	0xac8450,		// A6
	0xc09c68,		// A8
	0xd4b47c,		// AA
	0xe8cc90,		// AC
	0xfce0a4,		// AE
	0x2c3c00,		// B0
	0x485c1c,		// B2
	0x647c38,		// B4
	0x809c50,		// B6
	0x94b468,		// B8
	0xacd07c,		// BA
	0xc0e490,		// BC
	0xd4fca4,		// BE
	0x003c00,		// C0
	0x205c20,		// C2
	0x407c40,		// C4
	0x5c9c5c,		// C6
	0x74b474,		// C8
	0x8cd08c,		// CA
	0xa4e4a4,		// CC
	0xb8fcb8,		// CE
	0x003814,		// D0
	0x1c5c34,		// D2
	0x387c50,		// D4
	0x50986c,		// D6
	0x68b484,		// D8
	0x7ccc9c,		// DA
	0x90e4b4,		// DC
	0xa4fcc8,		// DE
	0x00302c,		// E0
	0x1c504c,		// E2
	0x347068,		// E4
	0x4c8c84,		// E6
	0x64a89c,		// E8
	0x78c0b4,		// EA
	0x88d4cc,		// EC
	0x9cece0,		// EE
	0x002844,		// F0
	0x184864,		// F2
	0x306884,		// F4
	0x4484a0,		// F6
	0x589cb8,		// F8
	0x6cb4d0,		// FA
	0x7ccce8,		// FC
	0x8ce0fc		// FE
};

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE g_hInstance;

HWND hDlgMain;
COLORREF screen[228][262];
int x, y;

uint8_t hSyncCur = 0x00;
uint8_t vSyncCur = 0x00;

DWORD WINAPI ShowDlgComposite(LPVOID lpParam)
{
	return DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DLG_MAIN), HWND_DESKTOP, DlgProc);
}

void OnTick(uint8_t data)
{
	screen[x][y] = RGB((uint8_t)(ntscRGB[data / 2] >> 0), (uint8_t)(ntscRGB[data / 2] >> 8), (uint8_t)(ntscRGB[data / 2] >> 16));
	x++;
}

void OnHSync(uint8_t data)
{
	if (data != hSyncCur)
	{
		hSyncCur = data;
		if (data != 0x00)
		{
			y++;
			x = 0;
		}
	}
}

void OnVSync(uint8_t data)
{
	if (data != vSyncCur)
	{
		vSyncCur = data;
		if (data != 0x00)
		{
			y = 0;
			x = 0;
		}
	}
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
