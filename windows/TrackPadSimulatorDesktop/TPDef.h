#pragma once

/** 
 * 
 * Packet Define
 * 
 * HEAD		:2b	:F1 F1
 * TYPE		:2b	:The type of packet.
 *			0000:Normal. The first packet of this type does not contain DATAID, INDEX.
 *				 If the data is divided to several data packets,
 *				 these packets do not contain HEAD, TYPE, LENGTH, DATAID, INDEX, CHECKSUM, END, just DATA.
 *				 So this type does not support resuming download.
 *			0001:*The whole data will be divided to several data packets which contain HEAD, TYPE, LENGTH, DATAID, INDEX, CHECKSUM and END.
 *				 This type can be used for resuming download if the server implements it.
 * LENGTH	:4b	:The length of *DATAID, *INDEX, CHECKSUM, DATA and END.
 * DATAID	:16b:*The identity of the whole data. In general, the identity is MD5. Used to distinguish two totally different data of files.
 * INDEX	:8b :*The index of the data. If this packet's data is broken, the server will send the index to the client and requires resending this packet.
 * CHECKSUM	:4b	:The checksum of HEAD, TYPE, LENGTH, *DATAID and *INDEX.
 * DATA		:Nb	:Custom data structure.
 * END		:2b	:F5 F5
 *
 **/

/**
 * Mouse Data Packet Define
 * INPUT TYPE		:1b
 * ACTION			:1b
 * POINTER COUNT	:4b
 * MOVE DISTANCE X	:4b:Available when action is ACTION_MOVE
 * MOVE DISTANCE Y	:4b:Available when action is ACTION_MOVE
 */

/**
 * Keyboard Data Packet Define
 * INPUT TYPE	:1b
 * ACTION		:1b
 * KEY			:4b
 * STATE		:1b:Availabel when action is ACTION_KK_TOGGLE. 0:OFF 1:ON.
 */

// Packet
#define PACKET_HEAD_SIZE		2
#define PACKET_TYPE_SIZE		2
#define PACKET_LENGTH_SIZE		4
#define PACKET_HEAD_LENGTH		(PACKET_HEAD_SIZE + PACKET_TYPE_SIZE + PACKET_LENGTH_SIZE)
#define PACKET_DATAID_SIZE		16
#define PACKET_INDEX_SIZE		8
#define PACKET_CHECKSUM_SIZE	4
#define PACKET_END_SIZE			2

#define HEAD_FLAG				(0xF1F1)
#define END_FLAG				(0xF5F5)

// 输入类型-鼠标
#define INPUT_TYPE_MOUSE		(0x00)
// 输入类型-键盘
#define INPUT_TYPE_KEYBOARD		(0x01)

// 按下，不论多少根手指
#define ACTION_DOWN				(0x01)
// 抬起，不区分单击抬起还是双击抬起
#define ACTION_UP				(0x02)
// 移动，不论多少根手指，不区分单击移动还是双击移动
#define ACTION_MOVE				(0x03)
// 单击，按下后迅速抬起
#define ACTION_SINGLE_TAP		(0x04)
// 单击抬起，按下后停顿（不是长按）再抬起，或者其他手指先抬起，最后一根手指再抬起
#define ACTION_SINGLE_TAP_UP	(0x05)
// 双击的第二下按下
#define ACTION_DOUBLE_TAP_DOWN	(0x06)
// 双击的第二下抬起
#define ACTION_DOUBLE_TAP_UP	(0x07)
// 双击的第二下按下后移动，移动中方向改变或者停顿将变成ACTION_MOVE
#define ACTION_DOUBLE_TAP_MOVE	(0x08)
// 单指迅速滑动
#define ACTION_FLING			(0x09)

// 键盘按键按下
#define ACTION_KK_DOWN			(0x0A)
// 键盘按键释放
#define ACTION_KK_UP			(0x0B)
// 键盘按键按下立即释放
#define ACTION_KK_PRESS			(0x0C)
// 键盘Caps Lock、Scroll Lock、Num Lock切换
#define ACTION_KK_TOGGLE		(0x0D) 

//
#define KK_UNKNOWN				(0x00)
// 
#define KK_BACKSPACE			(0x08)
#define KK_TAB					(0x09)
//
#define KK_ENTER				(0x0D)
//
#define KK_SHIFT				(0x10)
#define KK_CTRL					(0x11)
#define KK_ALT					(0x12)
#define KK_PAUSE_BREAK			(0x13)
#define KK_CAPS_LOCK			(0x14)
//
#define KK_ESC					(0x1B)
//
#define KK_SPACE				(0x20)
#define KK_PAGE_UP				(0x21)
#define KK_PAGE_DOWN			(0x22)
//
#define KK_END					(0x23)
#define KK_HOME					(0x24)
//
#define KK_LEFT					(0x25)
#define KK_UP					(0x26)
#define KK_RIGHT				(0x27)
#define KK_DOWN					(0x28)
//
#define KK_PRINT_SCREEN			(0x2C)
#define KK_INSERT				(0x2D)
#define KK_DELETE				(0x2E)
// KK_0 - KK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
#define KK_0					(0x30)
#define KK_1					(0x31)
#define KK_2					(0x32)
#define KK_3					(0x33)
#define KK_4					(0x34)
#define KK_5					(0x35)
#define KK_6					(0x36)
#define KK_7					(0x37)
#define KK_8					(0x38)
#define KK_9					(0x39)
// KK_A - KK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
#define KK_A					(0x41)
#define KK_B					(0x42)
#define KK_C					(0x43)
#define KK_D					(0x44)
#define KK_E					(0x45)
#define KK_F					(0x46)
#define KK_G					(0x47)
#define KK_H					(0x48)
#define KK_I					(0x49)
#define KK_J					(0x4A)
#define KK_K					(0x4B)
#define KK_L					(0x4C)
#define KK_M					(0x4D)
#define KK_N					(0x4E)
#define KK_O					(0x4F)
#define KK_P					(0x50)
#define KK_Q					(0x51)
#define KK_R					(0x52)
#define KK_S					(0x53)
#define KK_T					(0x54)
#define KK_U					(0x55)
#define KK_V					(0x56)
#define KK_W					(0x57)
#define KK_X					(0x58)
#define KK_Y					(0x59)
#define KK_Z					(0x5A)
// WIN, APPS - The menu shown when mouse right click.
#define KK_LWIN					(0x5B)
#define KK_RWIN					(0x5C)
#define KK_APPS					(0x5D)
// NUMPAD
#define KK_NUMPAD_0				(0x60)
#define KK_NUMPAD_1				(0x61)
#define KK_NUMPAD_2				(0x62)
#define KK_NUMPAD_3				(0x63)
#define KK_NUMPAD_4				(0x64)
#define KK_NUMPAD_5				(0x65)
#define KK_NUMPAD_6				(0x66)
#define KK_NUMPAD_7				(0x67)
#define KK_NUMPAD_8				(0x68)
#define KK_NUMPAD_9				(0x69)
#define KK_NUMPAD_MULTIPLY		(0x6A)
#define KK_NUMPAD_ADD			(0x6B)
#define KK_NUMPAD_SUBTRACT		(0x6D)
#define KK_NUMPAD_DECIMAL		(0x6E)
#define KK_NUMPAD_DIVIDE		(0x6F)
// Fn
#define KK_F1					(0x70)
#define KK_F2					(0x71)
#define KK_F3					(0x72)
#define KK_F4					(0x73)
#define KK_F5					(0x74)
#define KK_F6					(0x75)
#define KK_F7					(0x76)
#define KK_F8					(0x77)
#define KK_F9					(0x78)
#define KK_F10					(0x79)
#define KK_F11					(0x7A)
#define KK_F12					(0x7B)
//
#define KK_NUM_LOCK				(0x90)
#define KK_SCROLL_LOCK			(0x91)
//
#define KK_LSHIFT				(0xA0)
#define KK_RSHIFT				(0xA1)
#define KK_LCTRL				(0xA2)
#define KK_RCTRL				(0xA3)
#define KK_LALT					(0xA4)
#define KK_RALT					(0xA5)
//>> Windows 2000 or above
// Browser
#define KK_BROWSER_BACK			(0xA6)
#define KK_BROWSER_FORWARD		(0xA7)
#define KK_BROWSER_REFRESH		(0xA8)
#define KK_BROWSER_STOP			(0xA9)
#define KK_BROWSER_SEARCH		(0xAA)
#define KK_BROWSER_FAVORITES	(0xAB)
#define KK_BROWSER_HOME			(0xAC)
// Volume
#define KK_VOLUME_MUTE			(0xAD)
#define KK_VOLUME_DOWN			(0xAE)
#define KK_VOLUME_UP			(0xAF)
// Media
#define KK_MEDIA_NEXT_TRACK		(0xB0)
#define KK_MEDIA_PREV_TRACK    	(0xB1)
#define KK_MEDIA_STOP          	(0xB2)
#define KK_MEDIA_PLAY_PAUSE    	(0xB3)
// Launcher
#define KK_LAUNCH_MAIL         	(0xB4)
#define KK_LAUNCH_MEDIA_SELECT 	(0xB5)
#define KK_LAUNCH_APP1         	(0xB6)
#define KK_LAUNCH_APP2         	(0xB7)
//<<
// ;: =+ ,< -_ .> /? `~
#define KK_SEMI_COLON			(0xBA)
#define KK_EQUAL_PLUS			(0xBB)
#define KK_COMMA_LESS_THAN		(0xBC)
#define KK_MINUS_UNDERLINE		(0xBD)
#define KK_PERIOD_GREATER_THAN	(0xBE)
#define KK_SLASH_QUESTION_MARK	(0xBF)
#define KK_BACK_QUOTATION_TILDE	(0xC0)
// [{ \| ]} '"
#define KK_L_SQUARE_CURLY_BRACE (0xDB)
#define KK_BACKSLASH_V_BAR		(0xDC)
#define KK_R_SQUARE_CURLY_BRACE	(0xDD)
#define KK_QUOTATION_DOUBLE		(0xDE)
// 
#define KK_LOCK_SCREEN			(0xE0)
#define KK_SLEEP				(0xE1)
#define KK_RESTART				(0xE2)
#define KK_SHUTDOWN				(0xE3)

//
typedef struct _TP_ACTION
{
	char input_type;
	char action;
	union{
		char pointer_count;
		int keyboard_key;
	};
	union{
		int distance_x;
		char state;
	};
	int distance_y;
} TP_ACTION;

//
int MapKeyboardKey(int key);
int MapVirtualKey(int vkCode);