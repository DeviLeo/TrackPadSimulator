package org.devil.devileo.trackpadsimulator;

import android.inputmethodservice.KeyboardView;

public interface MainActivityInterface {
	public void setTextCoord(String coord);
	public void setTextGesture(String gesture);
	
	// Touch Event
	public int getPointerCount();
	public void sendData(byte[] data);
	
	// Keyboard
	public KeyboardView getKeyboardView();
	
	// Message
	public void onSocketStateChanged(int state);
	
	// Action
	public int ResolveAction(TPAction action);
}
