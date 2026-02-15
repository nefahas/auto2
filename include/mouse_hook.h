#pragma once
#include <windows.h>

extern volatile int toggled;
extern volatile int m1_down;
DWORD WINAPI init_hook_thread(LPVOID lpParam);