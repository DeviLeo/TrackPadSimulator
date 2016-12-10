#pragma once

#include "resource.h"

//>>

//
#define IDC_LABEL_COORD		101

//
HWND hWindow;
HHOOK hMouseHook;
HHOOK hKeyboardHook;
//

void CreateControls(HWND hWnd, HINSTANCE hInstance);
void CreateHooks(HINSTANCE hInstance);
__declspec(dllexport) LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
__declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

//<<
