#include "stdafx.h"

int MapKeyboardKey(int key)
{
	// Shutdown
	if		(key == KK_LOCK_SCREEN)	return KK_LOCK_SCREEN;
	else if (key == KK_SLEEP)		return KK_SLEEP;
	else if (key == KK_RESTART)		return KK_RESTART;
	else if (key == KK_SHUTDOWN)	return KK_SHUTDOWN;
	// Left Ctrl Alt Shift Win
	else if (key == KK_LCTRL)		return VK_LCONTROL;
	else if (key == KK_LALT)		return VK_LMENU;
	else if (key == KK_LSHIFT)		return VK_LSHIFT;
	else if (key == KK_LWIN)		return VK_LWIN;
	// Right Ctrl Alt Shift Win Apps
	else if (key == KK_RCTRL)		return VK_RCONTROL;
	else if (key == KK_RALT)		return VK_RMENU;
	else if (key == KK_RSHIFT)		return VK_RSHIFT;
	else if (key == KK_RWIN)		return VK_RWIN;
	else if (key == KK_APPS)		return VK_APPS;
	// NUMPAD
	else if (key == KK_NUMPAD_0)		return VK_NUMPAD0;
	else if (key == KK_NUMPAD_1)		return VK_NUMPAD1;
	else if (key == KK_NUMPAD_2)		return VK_NUMPAD2;
	else if (key == KK_NUMPAD_3)		return VK_NUMPAD3;
	else if (key == KK_NUMPAD_4)		return VK_NUMPAD4;
	else if (key == KK_NUMPAD_5)		return VK_NUMPAD5;
	else if (key == KK_NUMPAD_6)		return VK_NUMPAD6;
	else if (key == KK_NUMPAD_7)		return VK_NUMPAD7;
	else if (key == KK_NUMPAD_8)		return VK_NUMPAD8;
	else if (key == KK_NUMPAD_9)		return VK_NUMPAD9;
	else if (key == KK_NUMPAD_MULTIPLY)	return VK_MULTIPLY;
	else if (key == KK_NUMPAD_ADD)		return VK_ADD;
	else if (key == KK_NUMPAD_SUBTRACT)	return VK_SUBTRACT;
	else if (key == KK_NUMPAD_DECIMAL)	return VK_DECIMAL;
	else if (key == KK_NUMPAD_DIVIDE)	return VK_DIVIDE;
	// ESC Fn
	else if (key == KK_ESC)			return VK_ESCAPE;
	else if (key == KK_F1)			return VK_F1;
	else if (key == KK_F2)			return VK_F2;
	else if (key == KK_F3)			return VK_F3;
	else if (key == KK_F4)			return VK_F4;
	else if (key == KK_F5)			return VK_F5;
	else if (key == KK_F6)			return VK_F6;
	else if (key == KK_F7)			return VK_F7;
	else if (key == KK_F8)			return VK_F8;
	else if (key == KK_F9)			return VK_F9;
	else if (key == KK_F10)			return VK_F10;
	else if (key == KK_F11)			return VK_F11;
	else if (key == KK_F12)			return VK_F12;
	// 0 ~ 9
	else if (key == KK_1)			return '1';
	else if (key == KK_2)			return '2';
	else if (key == KK_3)			return '3';
	else if (key == KK_4)			return '4';
	else if (key == KK_5)			return '5';
	else if (key == KK_6)			return '6';
	else if (key == KK_7)			return '7';
	else if (key == KK_8)			return '8';
	else if (key == KK_9)			return '9';
	else if (key == KK_0)			return '0';
	// QWERT YUIOP
	else if (key == KK_Q)			return 'Q';
	else if (key == KK_W)			return 'W';
	else if (key == KK_E)			return 'E';
	else if (key == KK_R)			return 'R';
	else if (key == KK_T)			return 'T';
	else if (key == KK_Y)			return 'Y';
	else if (key == KK_U)			return 'U';
	else if (key == KK_I)			return 'I';
	else if (key == KK_O)			return 'O';
	else if (key == KK_P)			return 'P';
	// ASDFG HJKL
	else if (key == KK_A)			return 'A';
	else if (key == KK_S)			return 'S';
	else if (key == KK_D)			return 'D';
	else if (key == KK_F)			return 'F';
	else if (key == KK_G)			return 'G';
	else if (key == KK_H)			return 'H';
	else if (key == KK_J)			return 'J';
	else if (key == KK_K)			return 'K';
	else if (key == KK_L)			return 'L';
	// ZXCVBNM
	else if (key == KK_Z)			return 'Z';
	else if (key == KK_X)			return 'X';
	else if (key == KK_C)			return 'C';
	else if (key == KK_V)			return 'V';
	else if (key == KK_B)			return 'B';
	else if (key == KK_N)			return 'N';
	else if (key == KK_M)			return 'M';
	// SPACE
	else if (key == KK_SPACE)		return VK_SPACE;
	else if (key == KK_BACKSPACE)	return VK_BACK;
	// Tab Caps Lock Return
	else if (key == KK_TAB)			return VK_TAB;
	else if (key == KK_CAPS_LOCK)	return VK_CAPITAL;
	else if (key == KK_ENTER)		return VK_RETURN;
	// `~ -_ =+ [{ ]} \|
	else if (key == KK_BACK_QUOTATION_TILDE)return VK_OEM_3;
	else if (key == KK_MINUS_UNDERLINE)		return VK_OEM_MINUS;
	else if (key == KK_EQUAL_PLUS)			return VK_OEM_PLUS;
	else if (key == KK_L_SQUARE_CURLY_BRACE)return VK_OEM_4;
	else if (key == KK_R_SQUARE_CURLY_BRACE)return VK_OEM_6;
	else if (key == KK_BACKSLASH_V_BAR)		return VK_OEM_5;
	// ;: '" ,< .> /?
	else if (key == KK_SEMI_COLON)			return VK_OEM_1;
	else if (key == KK_QUOTATION_DOUBLE)	return VK_OEM_7;
	else if (key == KK_COMMA_LESS_THAN)		return VK_OEM_COMMA;
	else if (key == KK_PERIOD_GREATER_THAN)	return VK_OEM_PERIOD;
	else if (key == KK_SLASH_QUESTION_MARK)	return VK_OEM_2;
	// PrintScreen ScrollLock PauseBreak NumLock
	else if (key == KK_PRINT_SCREEN)		return VK_SNAPSHOT;
	else if (key == KK_SCROLL_LOCK)			return VK_SCROLL;
	else if (key == KK_PAUSE_BREAK)			return VK_PAUSE;
	else if (key == KK_NUM_LOCK)			return VK_NUMLOCK;
	// Insert Home Page Up
	else if (key == KK_INSERT)				return VK_INSERT;
	else if (key == KK_HOME)				return VK_HOME;
	else if (key == KK_PAGE_UP)				return VK_PRIOR;
	// Delete End Page Down
	else if (key == KK_DELETE)				return VK_DELETE;
	else if (key == KK_END)					return VK_END;
	else if (key == KK_PAGE_DOWN)			return VK_NEXT;
	// Left Up Right Down
	else if (key == KK_LEFT)				return VK_LEFT;
	else if (key == KK_UP)					return VK_UP;
	else if (key == KK_RIGHT)				return VK_RIGHT;
	else if (key == KK_DOWN)				return VK_DOWN;
	// Volume
	else if	(key == KK_VOLUME_MUTE)			return VK_VOLUME_MUTE;
	else if (key == KK_VOLUME_DOWN)			return VK_VOLUME_DOWN;
	else if (key == KK_VOLUME_UP)			return VK_VOLUME_UP;
	// Media
	else if (key == KK_MEDIA_PREV_TRACK)	return VK_MEDIA_PREV_TRACK;
	else if (key == KK_MEDIA_STOP)			return VK_MEDIA_STOP;
	else if (key == KK_MEDIA_PLAY_PAUSE)	return VK_MEDIA_PLAY_PAUSE;
	else if (key == KK_MEDIA_NEXT_TRACK)	return VK_MEDIA_NEXT_TRACK;
	// Launcher
	else if (key == KK_LAUNCH_MAIL)			return VK_LAUNCH_MAIL;
	else if (key == KK_LAUNCH_MEDIA_SELECT) return VK_LAUNCH_MEDIA_SELECT;
	else if (key == KK_LAUNCH_APP1)			return VK_LAUNCH_APP1;
	else if (key == KK_LAUNCH_APP2)			return VK_LAUNCH_APP2;

	return KK_UNKNOWN;
}

int MapVirtualKey(int key)
{
	// Shutdown
	if		(key == KK_LOCK_SCREEN)	return KK_LOCK_SCREEN;
	else if (key == KK_SLEEP)		return KK_SLEEP;
	else if (key == KK_RESTART)		return KK_RESTART;
	else if (key == KK_SHUTDOWN)	return KK_SHUTDOWN;
	// Left Ctrl Alt Shift Win
	else if (key == VK_LCONTROL)	return KK_LCTRL;
	else if (key == VK_LMENU)		return KK_LALT;
	else if (key == VK_LSHIFT)		return KK_LSHIFT;
	else if (key == VK_LWIN)		return KK_LWIN;
	// Right Ctrl Alt Shift Win Apps
	else if (key == VK_RCONTROL)	return KK_RCTRL;
	else if (key == VK_RMENU)		return KK_RALT;
	else if (key == VK_RSHIFT)		return KK_RSHIFT;
	else if (key == VK_RWIN)		return KK_RWIN;
	else if (key == VK_APPS)		return KK_APPS;
	// NUMPAD
	else if (key == VK_NUMPAD0)		return KK_NUMPAD_0;
	else if (key == VK_NUMPAD1)		return KK_NUMPAD_1;
	else if (key == VK_NUMPAD2)		return KK_NUMPAD_2;
	else if (key == VK_NUMPAD3)		return KK_NUMPAD_3;
	else if (key == VK_NUMPAD4)		return KK_NUMPAD_4;
	else if (key == VK_NUMPAD5)		return KK_NUMPAD_5;
	else if (key == VK_NUMPAD6)		return KK_NUMPAD_6;
	else if (key == VK_NUMPAD7)		return KK_NUMPAD_7;
	else if (key == VK_NUMPAD8)		return KK_NUMPAD_8;
	else if (key == VK_NUMPAD9)		return KK_NUMPAD_9;
	else if (key == VK_MULTIPLY)	return KK_NUMPAD_MULTIPLY;
	else if (key == VK_ADD)			return KK_NUMPAD_ADD;
	else if (key == VK_SUBTRACT)	return KK_NUMPAD_SUBTRACT;
	else if (key == VK_DECIMAL)		return KK_NUMPAD_DECIMAL;
	else if (key == VK_DIVIDE)		return KK_NUMPAD_DIVIDE;
	// ESC Fn
	else if (key == VK_ESCAPE)		return KK_ESC;
	else if (key == VK_F1)			return KK_F1;
	else if (key == VK_F2)			return KK_F2;
	else if (key == VK_F3)			return KK_F3;
	else if (key == VK_F4)			return KK_F4;
	else if (key == VK_F5)			return KK_F5;
	else if (key == VK_F6)			return KK_F6;
	else if (key == VK_F7)			return KK_F7;
	else if (key == VK_F8)			return KK_F8;
	else if (key == VK_F9)			return KK_F9;
	else if (key == VK_F10)			return KK_F10;
	else if (key == VK_F11)			return KK_F11;
	else if (key == VK_F12)			return KK_F12;
	// 0 ~ 9
	else if (key == '1')			return KK_1;
	else if (key == '2')			return KK_2;
	else if (key == '3')			return KK_3;
	else if (key == '4')			return KK_4;
	else if (key == '5')			return KK_5;
	else if (key == '6')			return KK_6;
	else if (key == '7')			return KK_7;
	else if (key == '8')			return KK_8;
	else if (key == '9')			return KK_9;
	else if (key == '0')			return KK_0;
	// QWERT YUIOP
	else if (key == 'Q')			return KK_Q;
	else if (key == 'W')			return KK_W;
	else if (key == 'E')			return KK_E;
	else if (key == 'R')			return KK_R;
	else if (key == 'T')			return KK_T;
	else if (key == 'Y')			return KK_Y;
	else if (key == 'U')			return KK_U;
	else if (key == 'I')			return KK_I;
	else if (key == 'O')			return KK_O;
	else if (key == 'P')			return KK_P;
	// ASDFG HJKL
	else if (key == 'A')			return KK_A;
	else if (key == 'S')			return KK_S;
	else if (key == 'D')			return KK_D;
	else if (key == 'F')			return KK_F;
	else if (key == 'G')			return KK_G;
	else if (key == 'H')			return KK_H;
	else if (key == 'J')			return KK_J;
	else if (key == 'K')			return KK_K;
	else if (key == 'L')			return KK_L;
	// ZXCVBNM
	else if (key == 'Z')			return KK_Z;
	else if (key == 'X')			return KK_X;
	else if (key == 'C')			return KK_C;
	else if (key == 'V')			return KK_V;
	else if (key == 'B')			return KK_B;
	else if (key == 'N')			return KK_N;
	else if (key == 'M')			return KK_M;
	// SPACE
	else if (key == VK_SPACE)		return KK_SPACE;
	else if (key == VK_BACK)		return KK_BACKSPACE;
	// Tab Caps Lock Return
	else if (key == VK_TAB)			return KK_TAB;
	else if (key == VK_CAPITAL)		return KK_CAPS_LOCK;
	else if (key == VK_RETURN)		return KK_ENTER;
	// `~ -_ =+ [{ ]} \|
	else if (key == VK_OEM_3)		return KK_BACK_QUOTATION_TILDE;
	else if (key == VK_OEM_MINUS)	return KK_MINUS_UNDERLINE;
	else if (key == VK_OEM_PLUS)	return KK_EQUAL_PLUS;
	else if (key == VK_OEM_4)		return KK_L_SQUARE_CURLY_BRACE;
	else if (key == VK_OEM_6)		return KK_R_SQUARE_CURLY_BRACE;
	else if (key == VK_OEM_5)		return KK_BACKSLASH_V_BAR;
	// ;: '" ,< .> /?
	else if (key == VK_OEM_1)		return KK_SEMI_COLON;
	else if (key == VK_OEM_7)		return KK_QUOTATION_DOUBLE;
	else if (key == VK_OEM_COMMA)	return KK_COMMA_LESS_THAN;
	else if (key == VK_OEM_PERIOD)	return KK_PERIOD_GREATER_THAN;
	else if (key == VK_OEM_2)		return KK_SLASH_QUESTION_MARK;
	// PrintScreen ScrollLock PauseBreak NumLock
	else if (key == VK_SNAPSHOT)	return KK_PRINT_SCREEN;
	else if (key == VK_SCROLL)		return KK_SCROLL_LOCK;
	else if (key == VK_PAUSE)		return KK_PAUSE_BREAK;
	else if (key == VK_NUMLOCK)		return KK_NUM_LOCK;
	// Insert Home Page Up
	else if (key == VK_INSERT)		return KK_INSERT;
	else if (key == VK_HOME)		return KK_HOME;
	else if (key == VK_PRIOR)		return KK_PAGE_UP;
	// Delete End Page Down
	else if (key == VK_DELETE)		return KK_DELETE;
	else if (key == VK_END)			return KK_END;
	else if (key == VK_NEXT)		return KK_PAGE_DOWN;
	// Left Up Right Down
	else if (key == VK_LEFT)		return KK_LEFT;
	else if (key == VK_UP)			return KK_UP;
	else if (key == VK_RIGHT)		return KK_RIGHT;
	else if (key == VK_DOWN)		return KK_DOWN;
	// Volume
	else if	(key == VK_VOLUME_MUTE)	return KK_VOLUME_MUTE;
	else if (key == VK_VOLUME_DOWN)	return KK_VOLUME_DOWN;
	else if (key == VK_VOLUME_UP)	return KK_VOLUME_UP;
	// Media
	else if (key == VK_MEDIA_PREV_TRACK)	return KK_MEDIA_PREV_TRACK;
	else if (key == VK_MEDIA_STOP)			return KK_MEDIA_STOP;
	else if (key == VK_MEDIA_PLAY_PAUSE)	return KK_MEDIA_PLAY_PAUSE;
	else if (key == VK_MEDIA_NEXT_TRACK)	return KK_MEDIA_NEXT_TRACK;
	// Launcher
	else if (key == VK_LAUNCH_MAIL)			return KK_LAUNCH_MAIL;
	else if (key == VK_LAUNCH_MEDIA_SELECT) return KK_LAUNCH_MEDIA_SELECT;
	else if (key == VK_LAUNCH_APP1)			return KK_LAUNCH_APP1;
	else if (key == VK_LAUNCH_APP2)			return KK_LAUNCH_APP2;

	return KK_UNKNOWN;
}