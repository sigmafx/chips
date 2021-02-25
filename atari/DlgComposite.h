#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>

DWORD WINAPI ShowDlgComposite(LPVOID lpParam);
void OnTick(uint8_t data);
void OnHSync(uint8_t data);
void OnVSync(uint8_t data);
