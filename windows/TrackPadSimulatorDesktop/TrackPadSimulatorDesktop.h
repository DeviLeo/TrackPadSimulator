#pragma once

#include "resource.h"

//
void Exit();
void CreateHooks(HINSTANCE hInstance);
__declspec(dllexport) LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
__declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);