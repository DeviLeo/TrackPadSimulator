package cn.devileo.trackpadsimulator;

/** 
 * 
 * Packet Define
 * 
 * HEAD		:2b	:F1 F1
 * TYPE		:2b	:The type of packet.
 *			0000:Normal. The first packet of this type does not contain DATAID, INDEX.
 * 				 If the data is divided to several data packets,
 * 				 these packets do not contain HEAD, TYPE, LENGTH, DATAID, INDEX, CHECKSUM, END, just DATA.
 * 				 So this type does not support resuming download.
 *			0001:*The whole data will be divided to several data packets which contain HEAD, TYPE, LENGTH, DATAID, INDEX, CHECKSUM and END.
 * 				 This type can be used for resuming download if the server implements it.
 * LENGTH	:4b	:The length of *DATAID, *INDEX, CHECKSUM, DATA and END.
 * DATAID	:16b:*The identity of the whole data. In general, the identity is MD5. Used to distinguish two totally different data of files.
 * INDEX	:8b :*The index of the data. If this packet's data is broken, the server will send the index to the client and requires resending this packet.
 * CHECKSUM	:4b	:The checksum of HEAD, TYPE, LENGTH, *DATAID and *INDEX.
 * DATA		:Nb	:Custom data structure.
 * END		:2b	:F5 F5
 *
 */

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

public final class Common {

	// Socket State
	public static final int SOCKET_STATE_UNKNOWN = 0;
	public static final int SOCKET_STATE_CONNECTING = 1;
	public static final int SOCKET_STATE_CONNECTED = 2;
	public static final int SOCKET_STATE_DISCONNECTED = 3;
	
	// Packet
	public static final int PACKET_HEAD_SIZE	= 2;
	public static final int PACKET_TYPE_SIZE	= 2;
	public static final int PACKET_LENGTH_SIZE	= 4;
	public static final int PACKET_HEAD_LENGTH	= (PACKET_HEAD_SIZE + PACKET_TYPE_SIZE + PACKET_LENGTH_SIZE);
	public static final int PACKET_DATAID_SIZE	= 16;
	public static final int PACKET_INDEX_SIZE	= 8;
	public static final int PACKET_CHECKSUM_SIZE= 4;
	public static final int PACKET_END_SIZE		= 2;
	
	public static final int HEAD_FLAG 	= 0xF1F1;	
	public static final int END_FLAG 	= 0xF5F5;
	
	public static final byte INPUT_TYPE_MOUSE 		= 0x00;	// 输入类型-鼠标
	public static final byte INPUT_TYPE_KEYBOARD 	= 0x01;	// 输入类型-键盘
	
	public static final byte ACTION_DOWN 			= 0x01;	// 按下，不论多少根手指
	public static final byte ACTION_UP 				= 0x02;	// 抬起，不区分单击抬起还是双击抬起
	public static final byte ACTION_MOVE 			= 0x03;	// 移动，不论多少根手指，不区分单击移动还是双击移动
	public static final byte ACTION_SINGLE_TAP 		= 0x04;	// 单击，按下后迅速抬起
	public static final byte ACTION_SINGLE_TAP_UP 	= 0x05;	// 单击抬起，按下后停顿（不是长按）再抬起，或者其他手指先抬起，最后一根手指再抬起
	public static final byte ACTION_DOUBLE_TAP_DOWN	= 0x06;	// 双击的第二下按下
	public static final byte ACTION_DOUBLE_TAP_UP 	= 0x07;	// 双击的第二下抬起
	public static final byte ACTION_DOUBLE_TAP_MOVE	= 0x08;	// 双击的第二下按下后移动，移动中方向改变或者停顿将变成ACTION_MOVE
	public static final byte ACTION_FLING 			= 0x09;	// 单指迅速滑动
	
	public static final byte ACTION_KK_DOWN 		= 0x0A;	// 键盘按键按下
	public static final byte ACTION_KK_UP 			= 0x0B;	// 键盘按键释放
	public static final byte ACTION_KK_PRESS 		= 0x0C;	// 键盘按键按下立即释放
	public static final byte ACTION_KK_TOGGLE		= 0x0D; // 键盘Caps Lock、Scroll Lock、Num Lock切换
	
	// Keyboard Key
	public static final int KK_UNKNOWN		= 0x00;

	// Mode Key
	public static final int KMK_ABC 				= -1;
	public static final int KMK_NUMBER_PUNCTUATION 	= -2;
	public static final int KMK_FN 					= -3;
	public static final int KMK_NUMPAD 				= -4;
	public static final int KMK_MEDIA	 			= -5;
	public static final int KMK_SHOW_KEYBOARD		= -101;
	public static final int KMK_HIDE_KEYBOARD		= -102;
	
	// 
	public static final int KK_BACKSPACE	= 0x08;
	public static final int KK_TAB			= 0x09;
	//
	public static final int KK_ENTER		= 0x0D;
	//
	public static final int KK_SHIFT		= 0x10;
	public static final int KK_CTRL			= 0x11;
	public static final int KK_ALT			= 0x12;
	public static final int KK_PAUSE_BREAK	= 0x13;
	public static final int KK_CAPS_LOCK	= 0x14;
	//
	public static final int KK_ESC			= 0x1B;
	//
	public static final int KK_SPACE		= 0x20;
	public static final int KK_PAGE_UP		= 0x21;
	public static final int KK_PAGE_DOWN	= 0x22;
	//
	public static final int KK_END			= 0x23;
	public static final int KK_HOME			= 0x24;
	// ← ↑ → ↓
	public static final int KK_LEFT			= 0x25;
	public static final int KK_UP			= 0x26;
	public static final int KK_RIGHT		= 0x27;
	public static final int KK_DOWN			= 0x28;
	//
	public static final int KK_PRINT_SCREEN	= 0x2C;
	public static final int KK_INSERT		= 0x2D;
	public static final int KK_DELETE		= 0x2E;
	// KK_0 - KK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
	public static final int KK_0	= 0x30;
	public static final int KK_1	= 0x31;
	public static final int KK_2	= 0x32;
	public static final int KK_3	= 0x33;
	public static final int KK_4	= 0x34;
	public static final int KK_5	= 0x35;
	public static final int KK_6	= 0x36;
	public static final int KK_7	= 0x37;
	public static final int KK_8	= 0x38;
	public static final int KK_9	= 0x39;
	// KK_A - KK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
	public static final int KK_A	= 0x41;
	public static final int KK_B	= 0x42;
	public static final int KK_C	= 0x43;
	public static final int KK_D	= 0x44;
	public static final int KK_E	= 0x45;
	public static final int KK_F	= 0x46;
	public static final int KK_G	= 0x47;
	public static final int KK_H	= 0x48;
	public static final int KK_I	= 0x49;
	public static final int KK_J	= 0x4A;
	public static final int KK_K	= 0x4B;
	public static final int KK_L	= 0x4C;
	public static final int KK_M	= 0x4D;
	public static final int KK_N	= 0x4E;
	public static final int KK_O	= 0x4F;
	public static final int KK_P	= 0x50;
	public static final int KK_Q	= 0x51;
	public static final int KK_R	= 0x52;
	public static final int KK_S	= 0x53;
	public static final int KK_T	= 0x54;
	public static final int KK_U	= 0x55;
	public static final int KK_V	= 0x56;
	public static final int KK_W	= 0x57;
	public static final int KK_X	= 0x58;
	public static final int KK_Y	= 0x59;
	public static final int KK_Z	= 0x5A;
	// WIN, APPS - The menu shown when mouse right click.
	public static final int KK_LWIN	= 0x5B;
	public static final int KK_RWIN	= 0x5C;
	public static final int KK_APPS	= 0x5D;
	// NUMPAD
	public static final int KK_NUMPAD_0		= 0x60;
	public static final int KK_NUMPAD_1		= 0x61;
	public static final int KK_NUMPAD_2		= 0x62;
	public static final int KK_NUMPAD_3		= 0x63;
	public static final int KK_NUMPAD_4		= 0x64;
	public static final int KK_NUMPAD_5		= 0x65;
	public static final int KK_NUMPAD_6		= 0x66;
	public static final int KK_NUMPAD_7		= 0x67;
	public static final int KK_NUMPAD_8		= 0x68;
	public static final int KK_NUMPAD_9		= 0x69;
	public static final int KK_NUMPAD_MULTIPLY	= 0x6A;
	public static final int KK_NUMPAD_ADD		= 0x6B;
	public static final int KK_NUMPAD_SUBTRACT	= 0x6D;
	public static final int KK_NUMPAD_DECIMAL	= 0x6E;
	public static final int KK_NUMPAD_DIVIDE	= 0x6F;
	// Fn
	public static final int KK_F1	= 0x70;
	public static final int KK_F2	= 0x71;
	public static final int KK_F3	= 0x72;
	public static final int KK_F4	= 0x73;
	public static final int KK_F5	= 0x74;
	public static final int KK_F6	= 0x75;
	public static final int KK_F7	= 0x76;
	public static final int KK_F8	= 0x77;
	public static final int KK_F9	= 0x78;
	public static final int KK_F10	= 0x79;
	public static final int KK_F11	= 0x7A;
	public static final int KK_F12	= 0x7B;
	//
	public static final int KK_NUM_LOCK		= 0x90;
	public static final int KK_SCROLL_LOCK	= 0x91;
	//
	public static final int KK_LSHIFT	= 0xA0;
	public static final int KK_RSHIFT	= 0xA1;
	public static final int KK_LCTRL	= 0xA2;
	public static final int KK_RCTRL	= 0xA3;
	public static final int KK_LALT		= 0xA4;
	public static final int KK_RALT		= 0xA5;
	
	//>> Windows 2000 or above
	// Browser
	public static final int KK_BROWSER_BACK		= 0xA6;
	public static final int KK_BROWSER_FORWARD	= 0xA7;
	public static final int KK_BROWSER_REFRESH	= 0xA8;
	public static final int KK_BROWSER_STOP		= 0xA9;
	public static final int KK_BROWSER_SEARCH	= 0xAA;
	public static final int KK_BROWSER_FAVORITES= 0xAB;
	public static final int KK_BROWSER_HOME		= 0xAC;
	
	// Volume
	public static final int KK_VOLUME_MUTE 		= 0xAD;
	public static final int KK_VOLUME_DOWN 		= 0xAE;
	public static final int KK_VOLUME_UP 		= 0xAF;
	
	// Media
	public static final int KK_MEDIA_NEXT_TRACK = 0xB0;
	public static final int KK_MEDIA_PREV_TRACK = 0xB1;
	public static final int KK_MEDIA_STOP 		= 0xB2;
	public static final int KK_MEDIA_PLAY_PAUSE = 0xB3;
	
	// Launcher
	public static final int KK_LAUNCH_MAIL 			= 0xB4;
	public static final int KK_LAUNCH_MEDIA_SELECT	= 0xB5;
	public static final int KK_LAUNCH_APP1 			= 0xB6;
	public static final int KK_LAUNCH_APP2 			= 0xB7;
	//<<
	
	// ;: =+ ,< -_ .> /? `~
	public static final int KK_SEMI_COLON			= 0xBA;
	public static final int KK_EQUAL_PLUS			= 0xBB;
	public static final int KK_COMMA_LESS_THAN		= 0xBC;
	public static final int KK_MINUS_UNDERLINE		= 0xBD;
	public static final int KK_PERIOD_GREATER_THAN	= 0xBE;
	public static final int KK_SLASH_QUESTION_MARK	= 0xBF;
	public static final int KK_BACK_QUOTATION_TILDE	= 0xC0;
	// [{ \| ]} '"
	public static final int KK_L_SQUARE_CURLY_BRACE	= 0xDB;
	public static final int KK_BACKSLASH_V_BAR		= 0xDC;
	public static final int KK_R_SQUARE_CURLY_BRACE	= 0xDD;
	public static final int KK_QUOTATION_DOUBLE		= 0xDE;
	
	//
	public static final int KK_LOCK_SCREEN	= 0xE0;
	public static final int KK_SLEEP		= 0xE1;
	public static final int KK_RESTART		= 0xE2;
	public static final int KK_SHUTDOWN		= 0xE3;
}

