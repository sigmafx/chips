#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "DlgComposite.h"
#include "Atari.h"

HINSTANCE g_hInstance;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE /* hPrevInstance */,
	_In_ LPWSTR    /* lpCmdLine */,
	_In_ int       /* nCmdShow */)
{
	HANDLE hThread;
	DWORD dwThreadId;

	g_hInstance = hInstance;
	
	hThread = CreateThread(
		NULL,
		0,
		Start,
		NULL,
		0,
		&dwThreadId);
	if (hThread != NULL)
	{
		ShowDlgComposite(NULL);
	}

	return 0;
}
