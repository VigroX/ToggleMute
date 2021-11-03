#include "mictoggler.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

HHOOK hhkLowLevelKybd;

LRESULT CALLBACK
keyboardHook(int code, WPARAM wParam, LPARAM lParam) {
	if (code != HC_ACTION)
		return CallNextHookEx(0, code, wParam, lParam);

	const KBDLLHOOKSTRUCT* event = (KBDLLHOOKSTRUCT*)lParam;

	if (event->vkCode == 181 && wParam == WM_KEYDOWN) {
		toggleMuteState();
		return S_FALSE;
	}
	return CallNextHookEx(0, code, wParam, lParam);
}

int WinMain() {

	HINSTANCE instance = GetModuleHandle(NULL);

	hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, instance, 0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hhkLowLevelKybd);

	return 0;
}