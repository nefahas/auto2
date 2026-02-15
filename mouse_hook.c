#include "include/mouse_hook.h"

volatile int toggled = 0;
volatile int m1_down = 0;

LRESULT CALLBACK mouse_proc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT *mouse = (MSLLHOOKSTRUCT *)lParam;

        if (mouse->flags & LLMHF_INJECTED) {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        if (wParam == WM_MBUTTONDOWN) {
            toggled = !toggled;
            return 1;
        } else if (wParam == WM_LBUTTONDOWN) {
            m1_down = 1;
        } else if (wParam == WM_LBUTTONUP) {
            m1_down = 0;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int init_hook() {
    HHOOK hook = SetWindowsHookEx(
        WH_MOUSE_LL,
        mouse_proc,
        GetModuleHandle(NULL),
        0
    );

    if (!hook) {
        return 0;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);
    return 1;
}

DWORD WINAPI init_hook_thread(LPVOID lpParam) {
    return init_hook();
}