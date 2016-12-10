#include "stdafx.h"

BOOL g_bWheel = FALSE;

// 按下，不论多少根手指
int OnActionDown(TP_ACTION action)
{
	if (ACTION_DOWN != action.action) return -ACTION_DOWN;
	
	g_bWheel = FALSE;

	//INPUT input = {0};
	//input.type = INPUT_MOUSE;
	//input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	//SendInput(1, &input, sizeof(INPUT));

	return ACTION_DOWN;
}
// 抬起，不区分单击抬起还是双击抬起
int OnActionUp(TP_ACTION action)
{
	if (ACTION_UP != action.action) return -ACTION_UP;
	
	//INPUT input = {0};
	//input.type = INPUT_MOUSE;
	//switch(action.pointer_count)
	//{
	//case 2:
	//	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
	//	break;
	//case 1:
	//default:
	//	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	//	break;
	//}
	//
	//SendInput(1, &input, sizeof(INPUT));

	return ACTION_UP;
}
// 移动，不论多少根手指，不区分单击移动还是双击移动
int OnActionMove(TP_ACTION action)
{
	if (ACTION_MOVE != action.action) return -ACTION_MOVE;
	
	if (2 == action.pointer_count)
	{
		DWORD dwDeltaV = 0, dwDeltaH = 0;
		int threshold = 2;
		if (action.distance_y > threshold) { dwDeltaV = WHEEL_DELTA; }
		else if (action.distance_y < -threshold) { dwDeltaV = -WHEEL_DELTA; }
		if (action.distance_x > threshold) { dwDeltaH = -WHEEL_DELTA; }
		else if (action.distance_x < -threshold) { dwDeltaH = WHEEL_DELTA; }

		if (dwDeltaV != 0)
		{
			INPUT inputV = {0};
			inputV.type = INPUT_MOUSE;
			inputV.mi.mouseData = dwDeltaV;
			inputV.mi.dwFlags = MOUSEEVENTF_WHEEL;
			SendInput(1, &inputV, sizeof(INPUT));

			g_bWheel = TRUE;
		}

		if (dwDeltaH != 0)
		{
			INPUT inputH = {0};
			inputH.type = INPUT_MOUSE;
			inputH.mi.mouseData = dwDeltaH;
			inputH.mi.dwFlags = MOUSEEVENTF_HWHEEL;
			SendInput(1, &inputH, sizeof(INPUT));

			g_bWheel = TRUE;
		}
	}
	// 这里使用g_bWheel来判断是因为，在双指滚动释放之后，会有相当高的几率触发单指的Touch Move事件，导致鼠标莫名其妙地跳跃。
	else if (!g_bWheel)
	{
		if (3 == action.pointer_count)
		{
			INPUT input = {0};
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, &input, sizeof(INPUT));
		}
	
		INPUT input = {0};
		input.type = INPUT_MOUSE;
		input.mi.dx = action.distance_x;
		input.mi.dy = action.distance_y;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(INPUT));
	}

	return ACTION_MOVE;
}
// 单击，按下后迅速抬起
int OnActionSingleTap(TP_ACTION action)
{
	if (ACTION_SINGLE_TAP != action.action) return -ACTION_SINGLE_TAP;

	INPUT input = {0};
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));

	return ACTION_SINGLE_TAP;
}
// 单击抬起，按下后停顿（不是长按）再抬起，或者其他手指先抬起，最后一根手指再抬起
int OnActionSingleTapUp(TP_ACTION action)
{
	if (ACTION_SINGLE_TAP_UP != action.action) return -ACTION_SINGLE_TAP_UP;

	BOOL act = TRUE;
	INPUT input = {0};
	input.type = INPUT_MOUSE;
	switch(action.pointer_count)
	{
	case 2:
		if (g_bWheel) { g_bWheel = FALSE; }
		else { input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP; }
		break;
	case 3:
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		break;
	case 1:
	default:
		act = FALSE;
		break;
	}

	if(act){
		SendInput(1, &input, sizeof(INPUT));
	}

	return ACTION_SINGLE_TAP_UP;
}
// 双击的第二下按下
int OnActionDoubleTapDown(TP_ACTION action)
{
	if (ACTION_DOUBLE_TAP_DOWN != action.action) return -ACTION_DOUBLE_TAP_DOWN;

	// Do nothing

	return ACTION_DOUBLE_TAP_DOWN;
}
// 双击的第二下抬起
int OnActionDoubleTapUp(TP_ACTION action)
{
	if (ACTION_DOUBLE_TAP_UP != action.action) return -ACTION_DOUBLE_TAP_UP;

	INPUT input[2] = {0};

	input[0].type = INPUT_MOUSE;
	input[1].type = INPUT_MOUSE;
	switch(action.pointer_count)
	{
	case 2:
		input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
		input[1].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
		break;
	case 1:
	default:
		input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
		input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
		break;
	}
	SendInput(2, input, sizeof(INPUT));

	return ACTION_DOUBLE_TAP_UP;
}
// 双击的第二下按下后移动，移动中方向改变或者停顿将变成ACTION_MOVE
int OnActionDoubleTapMove(TP_ACTION action)
{
	if (ACTION_DOUBLE_TAP_MOVE != action.action) return -ACTION_DOUBLE_TAP_MOVE;
	
	// Do nothing

	return ACTION_DOUBLE_TAP_MOVE;
}
// 单指迅速滑动
int OnActionFling(TP_ACTION action)
{
	if (ACTION_FLING != action.action) return -ACTION_FLING;
	
	// Do nothing

	return ACTION_FLING;
}

int OnActionKeyboardDown(TP_ACTION action)
{
	if (ACTION_KK_DOWN != action.action) return -ACTION_KK_DOWN;
	
	int key = MapKeyboardKey(action.keyboard_key);
	if (KK_UNKNOWN == key) { return -ACTION_KK_PRESS; }
	
	if (KK_LOCK_SCREEN != key &&
		KK_SLEEP != key &&
		KK_RESTART != key &&
		KK_SHUTDOWN != key)
	{
		INPUT input = {0};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = key;
		SendInput(1, &input, sizeof(INPUT));
	}

	return ACTION_KK_DOWN;
}
int OnActionKeyboardUp(TP_ACTION action)
{
	if (ACTION_KK_UP != action.action) return -ACTION_KK_UP;

	int key = MapKeyboardKey(action.keyboard_key);
	if (KK_UNKNOWN == key) { return -ACTION_KK_PRESS; }

	if (KK_LOCK_SCREEN == key)
	{
		LockWorkStation();
	}
	else if (KK_SLEEP == key ||
		KK_RESTART == key ||
		KK_SHUTDOWN == key)
	{
		Shutdown(key);
	}
	else
	{		
		INPUT input[2] = {0};

		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = key;
		
		input[1].type = INPUT_KEYBOARD;
		input[1].ki.dwFlags = KEYEVENTF_KEYUP;
		input[1].ki.wVk = key;
		SendInput(2, input, sizeof(INPUT));
	}

	return ACTION_KK_UP;
}
int OnActionKeyboardPress(TP_ACTION action)
{
	if (ACTION_KK_PRESS != action.action) return -ACTION_KK_PRESS;
	
	int key = MapKeyboardKey(action.keyboard_key);

	if (KK_UNKNOWN == key) { return -ACTION_KK_PRESS; }

	if (KK_LOCK_SCREEN == key)
	{
		LockWorkStation();
	}
	else if (KK_SLEEP == key ||
		KK_RESTART == key ||
		KK_SHUTDOWN == key)
	{
		Shutdown(key);
	}
	else
	{
		INPUT input[2] = {0};

		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = key;
		
		input[1].type = INPUT_KEYBOARD;
		input[1].ki.dwFlags = KEYEVENTF_KEYUP;
		input[1].ki.wVk = key;
		SendInput(2, input, sizeof(INPUT));
	}

	return ACTION_KK_PRESS;
}
int OnActionKeyboardToggle(TP_ACTION action)
{
	if (ACTION_KK_TOGGLE != action.action) return -ACTION_KK_TOGGLE;
	
	int key = MapKeyboardKey(action.keyboard_key);

	if (KK_UNKNOWN == key) { return -ACTION_KK_TOGGLE; }

	BOOL toggle = action.state == 1;

	// Do nothing

	return ACTION_KK_TOGGLE;
}

//
int Shutdown(int flag)
{
	UINT uFlag = 0;
	if (KK_SLEEP == flag) { } // Do after get privileges.
	else if (KK_RESTART == flag) { uFlag = EWX_REBOOT; }
	else if (KK_SHUTDOWN == flag) { uFlag = EWX_POWEROFF; }
	else { return -1; }

	if (!SetPrivilege(SE_SHUTDOWN_NAME, TRUE)) { return -2; }
	
	if (KK_SLEEP == flag)
	{
		if (!SetSystemPowerState(TRUE, TRUE)) { return -3; }
	}
	else
	{
		// Shut down the system and force all applications to close.
		if (!ExitWindowsEx(uFlag | EWX_FORCEIFHUNG,
			SHTDN_REASON_MAJOR_OTHER |
			SHTDN_REASON_MINOR_OTHER |
			SHTDN_REASON_FLAG_PLANNED)) { return -4; }
	}

	return 0;
}