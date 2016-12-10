package cn.devileo.trackpadsimulator.keyboard;

import java.util.HashMap;
import java.util.List;

import cn.devileo.trackpadsimulator.Common;

import android.inputmethodservice.Keyboard;
import android.inputmethodservice.Keyboard.Key;
import android.inputmethodservice.KeyboardView;

public final class KeyboardManager {
	
	// Just cache CTRL, SHIFT, ALT, WIN, CAPS LOCK, SCROLL LOCK, NUM LOCK
	// and all punctuation, we need the Key's objects of them.
	// Change the keyLabel of them when CapsLock or Shift is pressed.

	//>> KeyboardView
	public static KeyboardView kbdv = null;
	//<<
	
	//>> Keyboards
	public static Keyboard kbd_qwert 		= null;
	public static Keyboard kbd_qwert_shift 	= null;
	public static Keyboard kbd_fn 			= null;
	public static Keyboard kbd_numpad 		= null;
	public static Keyboard kbd_numpad_shift = null;
	public static Keyboard kbd_number_punctuation 		= null;
	public static Keyboard kbd_number_punctuation_shift = null;
	public static Keyboard kbd_media 		= null;
	public static Keyboard kbd_hidden		= null;
	
	private static Keyboard kbd_last_used 	= null;
	//<<
	
	//>> Keys
	public static HashMap<Keyboard, Key> key_ctrl 	= null;
	public static HashMap<Keyboard, Key> key_shift 	= null;
	public static HashMap<Keyboard, Key> key_alt 	= null;
	public static HashMap<Keyboard, Key> key_win 	= null;
	public static HashMap<Keyboard, Key> key_caps_lock 	= null;
	public static HashMap<Keyboard, Key> key_scroll_lock= null;
	public static HashMap<Keyboard, Key> key_num_lock 	= null;
	//<<
	
	//>> Control Keys State
	private static boolean key_state_ctrl_on 	= false;
	private static boolean key_state_shift_on 	= false;
	private static boolean key_state_alt_on 	= false;
	private static boolean key_state_win_on 	= false;
	private static boolean key_state_caps_lock_on 	= false;
	private static boolean key_state_scroll_lock_on = false;
	private static boolean key_state_num_lock_on 	= false;
	//<<
	
	public static final void setKeyboardABC() {
		if (key_state_shift_on || key_state_caps_lock_on) {
			kbdv.setKeyboard(kbd_qwert_shift);
		} else {
			kbdv.setKeyboard(kbd_qwert);
		}
	}

	public static final void setKeyboardNumberPunctuation() {
		if (key_state_shift_on) {
			kbdv.setKeyboard(kbd_number_punctuation_shift);
		} else {
			kbdv.setKeyboard(kbd_number_punctuation);
		}
	}

	public static final void setKeyboardFn() {
		kbdv.setKeyboard(kbd_fn);
	}

	public static final void setKeyboardNumpad() {
		if (key_state_num_lock_on) {
			kbdv.setKeyboard(kbd_numpad_shift);
		} else {
			kbdv.setKeyboard(kbd_numpad);
		}
	}
	
	public static final void setKeyboardMedia() {
		kbdv.setKeyboard(kbd_media);
	}
	
	public static final void hideKeyboard(){
		kbd_last_used = kbdv.getKeyboard();
		kbdv.setKeyboard(kbd_hidden);
	}
	
	public static final void showKeyboard(){
		kbdv.setKeyboard(kbd_last_used);
	}
	
	public static final void setKeyPressed(int key, boolean pressed) {
		boolean found = false;
		Keyboard kbd = kbdv.getKeyboard();
		Keyboard newkbd = kbd;
		found = setKeyPressed(kbd, key, pressed);
		if (!found && kbd_qwert != kbd) {
			newkbd = kbd_qwert;
			found = setKeyPressed(newkbd, key, pressed);
		}
		if (!found && kbd_qwert_shift != kbd) {
			newkbd = kbd_qwert_shift;
			found = setKeyPressed(newkbd, key, pressed);
		}
		if (!found && kbd_fn != kbd) {
			newkbd = kbd_fn;
			found = setKeyPressed(newkbd, key, pressed);
		}
		if (!found && kbd_numpad != kbd) {
			newkbd = kbd_numpad;
			found = setKeyPressed(newkbd, key, pressed);
		}
		if (!found && kbd_numpad_shift != kbd) {
			newkbd = kbd_numpad_shift;
			found = setKeyPressed(newkbd, key, pressed);
		}
		if (!found && kbd_number_punctuation != kbd) {
			newkbd = kbd_number_punctuation;
			found = setKeyPressed(newkbd, key, pressed);
		}
		if (!found && kbd_number_punctuation_shift != kbd) {
			newkbd = kbd_number_punctuation_shift;
			found = setKeyPressed(newkbd, key, pressed);
		}
		if (!found && kbd_media != kbd) {
			newkbd = kbd_media;
			found = setKeyPressed(newkbd, key, pressed);
		}
		if (found) {
			if (newkbd != kbd) {
				kbdv.setKeyboard(newkbd);
			} else {
				kbdv.invalidateAllKeys();
			}
		}
	}
	
	public static final boolean setKeyPressed(Keyboard kbd, int key, boolean pressed) {
		boolean found = false;
		List<Key> keys = kbd.getKeys();
		for(Key k : keys) {
			if (k.codes[0] == key) {
				k.pressed = pressed;
				found = true;
				break;
			}
		}
		return found;
	}
	
	//>> Set Key State
	public static final void setKeyStateOn(int key, boolean on) {
		switch(key) {
		case Common.KK_CTRL:
		case Common.KK_LCTRL:
		case Common.KK_RCTRL:
			setKeyStateCtrlOn(on);
			break;
		case Common.KK_SHIFT:
		case Common.KK_LSHIFT:
		case Common.KK_RSHIFT:
			setKeyStateShiftOn(on);
			break;
		case Common.KK_ALT:
		case Common.KK_LALT:
		case Common.KK_RALT:
			setKeyStateAltOn(on);
			break;
		case Common.KK_LWIN:
		case Common.KK_RWIN:
			setKeyStateWinOn(on);
			break;
		case Common.KK_CAPS_LOCK:
			setKeyStateCapsLockOn(on);
			break;
		case Common.KK_SCROLL_LOCK:
			setKeyStateScrollLockOn(on);
			break;
		case Common.KK_NUM_LOCK:
			setKeyStateNumLockOn(on);
			break;
		}
	}
	
	public static final void setKeyStateCtrlOn(boolean on) {
		key_state_ctrl_on = on;
		for(Key key : key_ctrl.values()) {
			key.on = on;
		}
		
		Keyboard curKeyboard = kbdv.getKeyboard();
		if (curKeyboard != kbd_media) {
			kbdv.invalidateAllKeys();
		}
	}
	
	public static final void setKeyStateShiftOn(boolean on) {
		key_state_shift_on = on;
		for(Key key : key_shift.values()) {
			key.on = on;
		}
		Keyboard curKeyboard = kbdv.getKeyboard();
		if (curKeyboard != kbd_media) {
			kbdv.invalidateAllKeys();
		}

		if (on) {
			// shift on, whether capslock on or off
			// qwert -> QWERT, 12345 -> !@#$%.
			if (curKeyboard == kbd_qwert) {
				kbdv.setKeyboard(kbd_qwert_shift);
			} else if (curKeyboard == kbd_number_punctuation) {
				kbdv.setKeyboard(kbd_number_punctuation_shift);
			}
		} else {
			// shift off, capslock on, no changes
			if (!key_state_caps_lock_on && curKeyboard == kbd_qwert_shift) {
				// shift off, capslock off
				// qwert <- QWERT
				kbdv.setKeyboard(kbd_qwert);
			} else if (curKeyboard == kbd_number_punctuation_shift) {
				// shift off, whether capslock on or off
				// 12345 <- !@#$%
				kbdv.setKeyboard(kbd_number_punctuation);
			}
		}
	}
	
	public static final void setKeyStateAltOn(boolean on) {
		key_state_ctrl_on = on;
		for(Key key : key_alt.values()) {
			key.on = on;
		}
		Keyboard curKeyboard = kbdv.getKeyboard();
		if (curKeyboard != kbd_media) {
			kbdv.invalidateAllKeys();
		}
	}
	
	public static final void setKeyStateWinOn(boolean on) {
		key_state_win_on = on;
		for(Key key : key_win.values()) {
			key.on = on;
		}
		Keyboard curKeyboard = kbdv.getKeyboard();
		if (curKeyboard != kbd_media) {
			kbdv.invalidateAllKeys();
		}
	}
	
	public static final void setKeyStateCapsLockOn(boolean on) {
		key_state_caps_lock_on = on;
		for(Key key : key_caps_lock.values()) {
			key.on = on;
		}
		Keyboard curKeyboard = kbdv.getKeyboard();
		if (curKeyboard == kbd_qwert ||
			curKeyboard == kbd_qwert_shift) {
			kbdv.invalidateAllKeys();
		}
		if (on) {
			// capslock off, whether shift on or off
			// qwert -> QWERT
			if (curKeyboard == kbd_qwert) {
				kbdv.setKeyboard(kbd_qwert_shift);
			}
		} else {
			// capslock off, shift on, no changes
			if (!key_state_shift_on && curKeyboard == kbd_qwert_shift) {
				// capslock off, shift off
				// qwert <- QWERT
				kbdv.setKeyboard(kbd_qwert);
			}
		}
	}
	
	public static final void setKeyStateScrollLockOn(boolean on) {
		key_state_scroll_lock_on = on;
		for(Key key : key_scroll_lock.values()) {
			key.on = on;
		}
		Keyboard curKeyboard = kbdv.getKeyboard();
		if (curKeyboard == kbd_fn) {
			kbdv.invalidateAllKeys();
		}
	}
	
	public static final void setKeyStateNumLockOn(boolean on) {
		key_state_num_lock_on = on;
		for(Key key : key_num_lock.values()) {
			key.on = on;
		}
		Keyboard curKeyboard = kbdv.getKeyboard();
		if (curKeyboard == kbd_numpad ||
			curKeyboard == kbd_numpad_shift) {
			kbdv.invalidateAllKeys();
		}
		if (on) {
			if (curKeyboard == kbd_numpad) {
				kbdv.setKeyboard(kbd_numpad_shift);
			}
		} else {
			if (curKeyboard == kbd_numpad_shift) {
				kbdv.setKeyboard(kbd_numpad);
			}
		}
	}
	//<
	
	public static final Key getKeyByCode(int primaryCode, boolean refreshKey) {
		Keyboard curKeyboard = kbdv.getKeyboard();
		Key key = null;
		switch(primaryCode) {
		case Common.KK_LCTRL:
			key = KeyboardManager.key_ctrl.get(curKeyboard);
			if (refreshKey) { setKeyStateCtrlOn(key.on); }
			break;
		case Common.KK_LSHIFT:
			key = KeyboardManager.key_shift.get(curKeyboard);
			if (refreshKey) { setKeyStateShiftOn(key.on); }
			break;
		case Common.KK_LALT:
			key = KeyboardManager.key_alt.get(curKeyboard);
			if (refreshKey) { setKeyStateAltOn(key.on); }
			break;
		case Common.KK_LWIN:
			key = KeyboardManager.key_win.get(curKeyboard);
			if (refreshKey) { setKeyStateWinOn(key.on); }
			break;
		case Common.KK_CAPS_LOCK:
			key = KeyboardManager.key_caps_lock.get(curKeyboard);
			if (refreshKey) { setKeyStateCapsLockOn(key.on); }
			break;
		case Common.KK_SCROLL_LOCK:
			key = KeyboardManager.key_scroll_lock.get(curKeyboard);
			if (refreshKey) { setKeyStateScrollLockOn(key.on); }
			break;
		case Common.KK_NUM_LOCK:
			key = KeyboardManager.key_num_lock.get(curKeyboard);
			if (refreshKey) { setKeyStateNumLockOn(key.on); }
			break;
		default:
			List<Key> keys = curKeyboard.getKeys();
			for(Key k : keys){
				if (k.codes[0] == primaryCode) {
					key = k;
					break;
				}
			}
		}
		return key;
	}
	
	public static final void cacheKeys(){
		HashMap<Keyboard, Key> hmCtrl 	= new HashMap<Keyboard, Keyboard.Key>(8);
		HashMap<Keyboard, Key> hmShift 	= new HashMap<Keyboard, Keyboard.Key>(8);
		HashMap<Keyboard, Key> hmAlt 	= new HashMap<Keyboard, Keyboard.Key>(8);
		HashMap<Keyboard, Key> hmWin 	= new HashMap<Keyboard, Keyboard.Key>(8);
		HashMap<Keyboard, Key> hmCapsLock 	= new HashMap<Keyboard, Keyboard.Key>(2);
		HashMap<Keyboard, Key> hmScrollLock = new HashMap<Keyboard, Keyboard.Key>(1);
		HashMap<Keyboard, Key> hmNumLock 	= new HashMap<Keyboard, Keyboard.Key>(2);
		
		cacheKeyboardKeys(kbd_qwert, hmCtrl, hmShift, hmAlt, hmWin, hmCapsLock, hmScrollLock, hmNumLock);
		cacheKeyboardKeys(kbd_qwert_shift, hmCtrl, hmShift, hmAlt, hmWin, hmCapsLock, hmScrollLock, hmNumLock);
		cacheKeyboardKeys(kbd_fn, hmCtrl, hmShift, hmAlt, hmWin, hmCapsLock, hmScrollLock, hmNumLock);
		cacheKeyboardKeys(kbd_numpad, hmCtrl, hmShift, hmAlt, hmWin, hmCapsLock, hmScrollLock, hmNumLock);
		cacheKeyboardKeys(kbd_numpad_shift, hmCtrl, hmShift, hmAlt, hmWin, hmCapsLock, hmScrollLock, hmNumLock);
		cacheKeyboardKeys(kbd_number_punctuation, hmCtrl, hmShift, hmAlt, hmWin, hmCapsLock, hmScrollLock, hmNumLock);
		cacheKeyboardKeys(kbd_number_punctuation_shift, hmCtrl, hmShift, hmAlt, hmWin, hmCapsLock, hmScrollLock, hmNumLock);
		
		key_ctrl 	= hmCtrl;
		key_shift 	= hmShift;
		key_alt 	= hmAlt;
		key_win 	= hmWin;
		key_caps_lock 	= hmCapsLock;
		key_scroll_lock = hmScrollLock;
		key_num_lock 	= hmNumLock;
	}
	
	private static final void cacheKeyboardKeys(Keyboard kbd, 
			HashMap<Keyboard, Key> hmCtrl, 
			HashMap<Keyboard, Key> hmShift, 
			HashMap<Keyboard, Key> hmAlt, 
			HashMap<Keyboard, Key> hmWin,
			HashMap<Keyboard, Key> hmCapsLock,
			HashMap<Keyboard, Key> hmScrollLock,
			HashMap<Keyboard, Key> hmNumLock) {
		List<Key> keys = kbd.getKeys();
		for (Key k : keys) {
			int code = k.codes[0];
			switch(code) {
			case Common.KK_LCTRL:
				hmCtrl.put(kbd, k);
				k.on = key_state_ctrl_on;
				break;
			case Common.KK_LSHIFT:
				hmShift.put(kbd, k);
				k.on = key_state_shift_on;
				break;
			case Common.KK_LALT:
				hmAlt.put(kbd, k);
				k.on = key_state_alt_on;
				break;
			case Common.KK_LWIN:
				hmWin.put(kbd, k);
				k.on = key_state_win_on;
				break;
			case Common.KK_CAPS_LOCK:
				hmCapsLock.put(kbd, k);
				k.on = key_state_caps_lock_on;
				break;
			case Common.KK_SCROLL_LOCK:
				hmScrollLock.put(kbd, k);
				k.on = key_state_scroll_lock_on;
				break;
			case Common.KK_NUM_LOCK:
				hmNumLock.put(kbd, k);
				k.on = key_state_num_lock_on;
				break;
			}
		}
	}
}
