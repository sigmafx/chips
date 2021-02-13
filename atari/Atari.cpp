#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include "Atari.h"

COLORREF screen[228][262];
HWND hDlgMain;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE /* hPrevInstance */,
                     _In_ LPWSTR    /* lpCmdLine */,
                     _In_ int       /* nCmdShow */)
{
    memset(screen, 0x7F, sizeof(DWORD) * 228 * 262);

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
