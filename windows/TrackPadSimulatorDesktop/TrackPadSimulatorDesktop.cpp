// TrackPadSimulatorDesktop.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

// Global Variables:
HINSTANCE hInst;	// current instance
HHOOK hMouseHook;
HHOOK hKeyboardHook;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	hInst = hInstance; // Store instance handle in our global variable
	CreateHooks(hInstance);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenThread, NULL, 0, NULL);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

//>>
void Exit()
{
	UnhookWindowsHookEx(hKeyboardHook);
	UnhookWindowsHookEx(hMouseHook);
	ExitProcess(NULL);
}

void CreateHooks(HINSTANCE hInstance)
{
	//hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, hInstance, 0);
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, hInstance, 0);
}

__declspec(dllexport) LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT *pMouseHookStruct = (MOUSEHOOKSTRUCT *)lParam;

	if (pMouseHookStruct != NULL)
	{
		if (wParam == WM_MOUSEMOVE)
		{
			//POINT ptPos = pMouseHookStruct->pt;
			//RECT rcClient;
			//GetClientRect(hWindow, &rcClient);
			//int sx = GetSystemMetrics(SM_CXSCREEN);
			//int sy = GetSystemMetrics(SM_CYSCREEN);
		}
	}

	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

__declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT *pKeyboardHookStruct = (KBDLLHOOKSTRUCT *)lParam;

	if (pKeyboardHookStruct != NULL)
	{
		DWORD vkCode = pKeyboardHookStruct->vkCode;
		short state = GetKeyState(vkCode);
		BOOL isKeyDown = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ? TRUE : FALSE;
		BOOL isKeyToggled = (state & 0x0001) ? TRUE : FALSE;
		int kkey = MapVirtualKey(vkCode);
		int ckey = KK_UNKNOWN;	// control key
		switch(vkCode)
		{
		case VK_CONTROL	:
		case VK_LCONTROL:
		case VK_RCONTROL:
		case VK_LSHIFT	:
		case VK_RSHIFT	:
		case VK_LMENU	:
		case VK_RMENU	:
		case VK_LWIN	:
		case VK_RWIN	:
			isKeyToggled = isKeyDown;
		case VK_CAPITAL	:
		case VK_SCROLL	:
		case VK_NUMLOCK	:
			ckey = kkey;
		}

		// Send Control Key Toggle State
		if (KK_UNKNOWN != ckey)
		{
			char data[7];
			data[0] = INPUT_TYPE_KEYBOARD;
			data[1] = ACTION_KK_TOGGLE;
			char *keyData = ConvertInt32ToChar(ckey);
			memcpy(data+2, keyData, 4);
			data[6] = isKeyToggled ? 1 : 0;
			SendData(data, 7);
		}
		
		// Send Key Pressed State
		char data[6];
		data[0] = INPUT_TYPE_KEYBOARD;
		data[1] = isKeyDown ? ACTION_KK_DOWN : ACTION_KK_UP;
		char *keyData = ConvertInt32ToChar(kkey);
		memcpy(data+2, keyData, 4);
		SendData(data, 6);
	}

	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

//<<