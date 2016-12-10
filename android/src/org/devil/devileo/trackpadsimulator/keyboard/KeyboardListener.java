package org.devil.devileo.trackpadsimulator.keyboard;

import java.nio.ByteBuffer;

import org.devil.devileo.trackpadsimulator.Common;
import org.devil.devileo.trackpadsimulator.MainActivityInterface;
import org.devil.devileo.trackpadsimulator.R;
import org.devil.devileo.trackpadsimulator.Util;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.inputmethodservice.Keyboard.Key;
import android.inputmethodservice.KeyboardView.OnKeyboardActionListener;
import android.util.Log;

public class KeyboardListener implements 
OnKeyboardActionListener {

	static final String LOG_TAG = "TrackPad_KeyboardListener";

	Context mContext;
	MainActivityInterface mDelegate;
	
	public KeyboardListener(Context context, MainActivityInterface delegate){
		super();
		mContext = context;
		mDelegate = delegate;
	}
	
	//>> OnKeyboardActionListener
	
	@Override
	public void onKey(int primaryCode, int[] keyCodes) {
		if (primaryCode > 0) {
			if (Common.KK_RESTART == primaryCode) {
				createRestartConfirmDialog();
			} else if (Common.KK_SHUTDOWN == primaryCode) {
				createShutdownConfirmDialog();
			} else if (Common.KK_SLEEP == primaryCode) {
				createSleepConfirmDialog();
			} else {
				Key key = KeyboardManager.getKeyByCode(primaryCode, true);
				byte action = Common.ACTION_KK_PRESS;
				if (key.modifier) {
					action = key.on ? Common.ACTION_KK_DOWN : Common.ACTION_KK_UP;
				}
				sendKey(action, primaryCode);
			}
		} else {
			switch(primaryCode) {
			case Common.KMK_ABC:
				KeyboardManager.setKeyboardABC();
				break;
			case Common.KMK_NUMBER_PUNCTUATION:
				KeyboardManager.setKeyboardNumberPunctuation();
				break;
			case Common.KMK_FN:
				KeyboardManager.setKeyboardFn();
				break;
			case Common.KMK_NUMPAD:
				KeyboardManager.setKeyboardNumpad();
				break;
			case Common.KMK_MEDIA:
				KeyboardManager.setKeyboardMedia();
				break;
			case Common.KMK_SHOW_KEYBOARD:
				KeyboardManager.showKeyboard();
				break;
			case Common.KMK_HIDE_KEYBOARD:
				KeyboardManager.hideKeyboard();
				break;
			}
		}
		
		StringBuilder log = new StringBuilder(keyCodes.length*8);
		for(int k : keyCodes){
			log.append(String.format("0x%02x ", k));
		}
		String gesture = "On Key " + primaryCode + " : " + primaryCode;
		mDelegate.setTextGesture(gesture);
		mDelegate.setTextCoord(log.toString());
	}

	@Override
	public void onPress(int primaryCode) {
		String gesture = "On Press " + primaryCode;
		mDelegate.setTextCoord(gesture);
	}

	@Override
	public void onRelease(int primaryCode) {
		String gesture = "On Release " + primaryCode;
		mDelegate.setTextCoord(gesture);
	}

	@Override
	public void onText(CharSequence text) {
	}

	@Override
	public void swipeDown() {
	}

	@Override
	public void swipeLeft() {
	}

	@Override
	public void swipeRight() {
	}

	@Override
	public void swipeUp() {
	}

	//<< OnKeyboardActionListener

	private void sendKey(byte action, int key){
		byte[] key_data = Util.convertInt32ToBytes(key);
		ByteBuffer bb = ByteBuffer.allocate(6);
		bb.put(Common.INPUT_TYPE_KEYBOARD);
		bb.put(action);
		bb.put(key_data);
		byte[] data = bb.array();
		mDelegate.sendData(data);
		
		String log = Util.convertBytes2String(data);
		Log.i(LOG_TAG, log);
		String gesture = "On Key " + action + " : " + key;
		mDelegate.setTextGesture(gesture);
		mDelegate.setTextCoord(log);
	}

	private void createSleepConfirmDialog(){
		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
		builder.setTitle(R.string.msg_confirm_title)
				.setIcon(android.R.drawable.ic_dialog_alert)
				.setMessage(R.string.msg_confirm_sleep)
				.setPositiveButton(R.string.dialog_button_ok, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						sendKey(Common.ACTION_KK_PRESS, Common.KK_SLEEP);
					}
				})
				.setNegativeButton(R.string.dialog_button_cancel, null);
		AlertDialog alert = builder.create();
		alert.show();
	}
	
	private void createRestartConfirmDialog(){
		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
		builder.setTitle(R.string.msg_confirm_title)
				.setIcon(android.R.drawable.ic_dialog_alert)
				.setMessage(R.string.msg_confirm_restart)
				.setPositiveButton(R.string.dialog_button_ok, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						sendKey(Common.ACTION_KK_PRESS, Common.KK_RESTART);
					}
				})
				.setNegativeButton(R.string.dialog_button_cancel, null);
		AlertDialog alert = builder.create();
		alert.show();
	}

	private void createShutdownConfirmDialog(){
		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
		builder.setTitle(R.string.msg_confirm_title)
				.setIcon(android.R.drawable.ic_dialog_alert)
				.setMessage(R.string.msg_confirm_shutdown)
				.setPositiveButton(R.string.dialog_button_ok, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						sendKey(Common.ACTION_KK_PRESS, Common.KK_SHUTDOWN);
					}
				})
				.setNegativeButton(R.string.dialog_button_cancel, null);
		AlertDialog alert = builder.create();
		alert.show();
	}
}
