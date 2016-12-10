// TrackPadSimulator.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TrackPadSimulator.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	if (!IsUserAdmin()) 
	{
		if(RunAsAdministrator())
		{
			return 0;
		}
	}
	else 
	{
		//TCHAR szFileName[] = TEXT("TrackPadSimulatorService.exe");
		//LaunchAsService(szFileName, sizeof(szFileName));
	}

	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TRACKPADSIMULATOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRACKPADSIMULATOR));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRACKPADSIMULATOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, 300, 130, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	//>>
	try
	{
		hWindow = hWnd;
		WTSRegisterSessionNotification(hWnd, NOTIFY_FOR_THIS_SESSION);
		CreateControls(hWnd, hInstance);
		CreateHooks(hInstance);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenThread, NULL, 0, NULL);
	}
	catch(std::exception &e)
	{
		MessageBoxA(NULL, e.what(), "ERROR", MB_OK|MB_ICONERROR);
		return EXIT_FAILURE;
	}
	//<<

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		//>>
	case WM_WTSSESSION_CHANGE:
		switch(wParam)
		{
		case WTS_CONSOLE_CONNECT:
			//MessageBox(hWnd, L"WTS_CONSOLE_CONNECT", NULL, MB_OK|MB_ICONINFORMATION);
			//break;
		case WTS_REMOTE_CONNECT:
			//MessageBox(hWnd, L"WTS_REMOTE_CONNECT", NULL, MB_OK|MB_ICONINFORMATION);
			//break;
		case WTS_SESSION_LOGON:
			//MessageBox(hWnd, L"WTS_SESSION_LOGON", NULL, MB_OK|MB_ICONINFORMATION);
			//break;
		case WTS_SESSION_UNLOCK:
			//MessageBox(hWnd, L"WTS_SESSION_UNLOCK", NULL, MB_OK|MB_ICONINFORMATION);
			g_bSendDataToDesktop = FALSE;
			break;
		case WTS_CONSOLE_DISCONNECT:
			//MessageBox(hWnd, L"WTS_CONSOLE_DISCONNECT", NULL, MB_OK|MB_ICONINFORMATION);
			//break;
		case WTS_REMOTE_DISCONNECT:
			//MessageBox(hWnd, L"WTS_REMOTE_DISCONNECT", NULL, MB_OK|MB_ICONINFORMATION);
			//break;
		case WTS_SESSION_LOGOFF:
			//MessageBox(hWnd, L"WTS_SESSION_LOGOFF", NULL, MB_OK|MB_ICONINFORMATION);
			//break;
		case WTS_SESSION_LOCK:
			//MessageBox(hWnd, L"WTS_SESSION_LOCK", NULL, MB_OK|MB_ICONINFORMATION);
			g_bSendDataToDesktop = TRUE;
			break;
		}
		break;
	case WM_SIZE:
		{
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);

			// Label
			POINT ptPos;
			GetCursorPos(&ptPos);
			int sx = GetSystemMetrics(SM_CXSCREEN);
			int sy = GetSystemMetrics(SM_CYSCREEN);
			HWND hLabel = GetDlgItem(hWnd, IDC_LABEL_COORD);
			SetWindowPos(hLabel, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
		}
		break;
		//<<
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		//>>
		DestroySocket();
		UnhookWindowsHookEx(hKeyboardHook);
		UnhookWindowsHookEx(hMouseHook);
		//<<
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		{
			//POINT ptPos;
			//GetCursorPos(&ptPos);
			//RECT rcClient;
			//GetClientRect(hWnd, &rcClient);
			//int sx = GetSystemMetrics(SM_CXSCREEN);
			//int sy = GetSystemMetrics(SM_CYSCREEN);
			//WCHAR coord[128];
			//wsprintf(coord, L"(%d,%d)\n(%d,%d)\n(%d,%d)", ptPos.x, ptPos.y, rcClient.right, rcClient.bottom, sx, sy);
			//HWND hLabel = GetDlgItem(hWnd, IDC_LABEL_COORD);
			//SetWindowText(hLabel, coord);
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
//>>

void CreateControls(HWND hWnd, HINSTANCE hInstance)
{
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);

	// Label
	HFONT hFontDefault;
	POINT ptPos;
	GetCursorPos(&ptPos);
	int sx = GetSystemMetrics(SM_CXSCREEN);
	int sy = GetSystemMetrics(SM_CYSCREEN);
	WCHAR coord[128];
	wsprintf(coord, L"(%d,%d)\n(%d,%d)\n(%d,%d)", ptPos.x, ptPos.y, rcClient.right, rcClient.bottom, sx, sy);
	HWND hLabel = CreateWindowEx(WS_EX_CLIENTEDGE, L"Static", coord,
		WS_CHILD|WS_VISIBLE|SS_CENTER, 0, 0, rcClient.right, rcClient.bottom, 
		hWnd, (HMENU)IDC_LABEL_COORD, GetModuleHandle(NULL), NULL);
	if(hLabel == NULL)
	{
		//MessageBox(hWnd, L"Cannot create label", L"Error", MB_OK|MB_ICONERROR);
		throw std::exception("无法创建文本\nCannot create label"); 
	}
	hFontDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hLabel, WM_SETFONT, (WPARAM)hFontDefault, MAKELPARAM(FALSE, 0));
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
			POINT ptPos = pMouseHookStruct->pt;
			RECT rcClient;
			GetClientRect(hWindow, &rcClient);
			int sx = GetSystemMetrics(SM_CXSCREEN);
			int sy = GetSystemMetrics(SM_CYSCREEN);
			WCHAR coord[128];
			wsprintf(coord, L"(%d,%d)\n(%d,%d)\n(%d,%d)", ptPos.x, ptPos.y, rcClient.right, rcClient.bottom, sx, sy);
			HWND hLabel = GetDlgItem(hWindow, IDC_LABEL_COORD);
			SetWindowText(hLabel, coord);
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

		int sx = GetSystemMetrics(SM_CXSCREEN);
		int sy = GetSystemMetrics(SM_CYSCREEN);
		WCHAR chKey = MapVirtualKey(vkCode, MAPVK_VK_TO_CHAR);
		WCHAR info[128];
		wsprintf(info, L"VK: %0.2x, %c, %x, %s, %s", vkCode, chKey == NULL ? ' ' : chKey, wParam,
			isKeyDown ? L"Down" : L"Up",
			isKeyToggled ? L"On" : L"Off");
		HWND hLabel = GetDlgItem(hWindow, IDC_LABEL_COORD);
		SetWindowText(hLabel, info);
	}

	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

//<<
