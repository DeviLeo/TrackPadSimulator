package cn.devileo.trackpadsimulator;

import java.nio.ByteBuffer;

import cn.devileo.trackpadsimulator.keyboard.KeyboardListener;
import cn.devileo.trackpadsimulator.keyboard.KeyboardManager;
import cn.devileo.trackpadsimulator.settings.TPSettingsActivity;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.inputmethodservice.Keyboard;
import android.inputmethodservice.KeyboardView;
import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements MainActivityInterface{
	
	//>> Static
	static final String LOG_TAG = "TrackPad_Activity";
	
	static int mOldOrientation = 0;
	//<<
	
	//>> Variables
	TextView tvCoord;
	TextView tvGesture;
	KeyboardView kvKeyboard;
	GestureDetector mGestureDetector;
	KeyboardListener mKeyboardListener;
	SocketClient mSocket;
	MotionEvent mPrevMotionEvent;
	int mMaxPointerCount;
	//<<

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mGestureDetector = new GestureDetector(this, new GestureListener(this, this));
		mKeyboardListener = new KeyboardListener(this, this);
		mPrevMotionEvent = null;
		mOldOrientation = getResources().getConfiguration().orientation;
		findAllViews();
		createKeyboard();
	}
	
	private void findAllViews(){
		tvCoord = (TextView)findViewById(R.id.tvCoord);
		tvGesture = (TextView)findViewById(R.id.tvGesture);
		kvKeyboard = (KeyboardView)findViewById(R.id.kvKeyboard);
	}
	
	public void createKeyboard(){
		KeyboardManager.kbdv = kvKeyboard;
		KeyboardManager.kbd_fn = new Keyboard(this, R.xml.tpkeyboard_portrait_fn);
		KeyboardManager.kbd_qwert = new Keyboard(this, R.xml.tpkeyboard_portrait_qwert);
		KeyboardManager.kbd_qwert_shift = new Keyboard(this, R.xml.tpkeyboard_portrait_qwert_shift);
		KeyboardManager.kbd_numpad = new Keyboard(this, R.xml.tpkeyboard_portrait_numpad);
		KeyboardManager.kbd_numpad_shift = new Keyboard(this, R.xml.tpkeyboard_portrait_numpad_shift);
		KeyboardManager.kbd_number_punctuation = new Keyboard(this, R.xml.tpkeyboard_portrait_number_punctuation);
		KeyboardManager.kbd_number_punctuation_shift = new Keyboard(this, R.xml.tpkeyboard_portrait_number_punctuation_shift);
		KeyboardManager.kbd_media = new Keyboard(this, R.xml.tpkeyboard_portrait_media);
		KeyboardManager.kbd_hidden = new Keyboard(this, R.xml.tpkeyboard_portrait_hidden);
		KeyboardManager.cacheKeys();
		
		kvKeyboard.setKeyboard(KeyboardManager.kbd_qwert);
		kvKeyboard.setOnKeyboardActionListener(mKeyboardListener);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		String action = "Touch Event";
		int actionMasked = event.getAction();
		if(MotionEvent.ACTION_DOWN == actionMasked){
			action = "Touch Down";
			onTouchDown(event);
		} else if(MotionEvent.ACTION_MOVE == actionMasked){
			action = "Touch Move";
			onTouchMove(event);
		} else if(MotionEvent.ACTION_UP == actionMasked){
			action = "Touch Up";
			onTouchUp(event);
		} else if(MotionEvent.ACTION_POINTER_DOWN == actionMasked){
			action = "Pointer Down";
		} else if(MotionEvent.ACTION_POINTER_UP == actionMasked){
			action = "Pointer Up";
		} else if(MotionEvent.ACTION_CANCEL == actionMasked){
			action = "Cancel";
		}
		String coord = event.getX() + "," + event.getY();
		String gesture = action + "(" + event.getAction() + ")" + ", " + event.getPointerCount() + " Point(s)";
		setTextCoord(coord);
		setTextGesture(gesture);
//		return super.onTouchEvent(event);
		return mGestureDetector.onTouchEvent(event);
	}
	
	private void onTouchDown(MotionEvent event){
		mPrevMotionEvent = null;
		mMaxPointerCount = event.getPointerCount();
		
		if (!isConnected()) return;
		
		byte[] data = new byte[3];
		data[0] = Common.INPUT_TYPE_MOUSE;
		data[1] = Common.ACTION_DOWN;
		data[2] = (byte)mMaxPointerCount;
		sendData(data);
		Log.i(LOG_TAG, Util.convertBytes2String(data));
	}
	
	private void onTouchUp(MotionEvent event){
		if (!isConnected()) return;
		if (mPrevMotionEvent != null) {
			mPrevMotionEvent = null;
		}
		byte[] data = new byte[3];
		data[0] = Common.INPUT_TYPE_MOUSE;
		data[1] = Common.ACTION_UP;
		data[2] = (byte)mMaxPointerCount;
		sendData(data);
		Log.i(LOG_TAG, Util.convertBytes2String(data));
	}
	
	private void onTouchMove(MotionEvent event){
		int curPointerCount = event.getPointerCount();
		if (curPointerCount > mMaxPointerCount){
			mMaxPointerCount = curPointerCount;
		}

		if (!isConnected()) return;
		
		if (null == mPrevMotionEvent){
			mPrevMotionEvent = MotionEvent.obtain(event);
			Log.d(LOG_TAG, mPrevMotionEvent +"|||"+event);
			Log.w(LOG_TAG, "Previous Motion Event is NULL!");
			return;
		}
		
		// One finger
		float curX = event.getX(), curY = event.getY();
		float prevX = mPrevMotionEvent.getX(), prevY = mPrevMotionEvent.getY();
		float disX = curX - prevX, disY = curY - prevY;
		int x = Math.round(disX), y = Math.round(disY);
		if (Math.abs(x) < 1 && Math.abs(y) < 1){
//			Log.w(LOG_TAG, String.format("DisX(%d) < 10 && DisY(%d) < 10", x, y));
			return;
		}
		byte[] xx = Util.convertInt32ToBytes(x);
		byte[] yy = Util.convertInt32ToBytes(y);
		
		ByteBuffer bb = ByteBuffer.allocate(11);
		bb.put(Common.INPUT_TYPE_MOUSE);
		bb.put(Common.ACTION_MOVE);
		bb.put((byte)event.getPointerCount());
		bb.put(xx);
		bb.put(yy);
		sendData(bb.array());

		Log.i(LOG_TAG, String.format("DisX,Y:%f,%f", disX, disY));
		
		mPrevMotionEvent = MotionEvent.obtain(event);
	}

	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		int itemid = item.getItemId();
		if (R.id.menu_action_start == itemid){
			// Start socket
			if (isConnected()){
				mSocket.disconnect();
				mSocket = null;
			}
			if (mSocket == null) {
				String pref_name = getString(R.string.pref_name);
				String pref_host_key = getString(R.string.tp_settings_pref_host);
				SharedPreferences sp = getSharedPreferences(pref_name, MODE_PRIVATE);
				String host = sp.getString(pref_host_key, null);
				Log.i(LOG_TAG, "To Connect: " + host);
				if(host == null){
					Toast.makeText(this, R.string.err_host_empty, Toast.LENGTH_LONG).show();
				} else {
					mSocket = new SocketClient(host, 22222);
					mSocket.setDelegate(this);
				}
			}
			if (mSocket != null) {
				if(mSocket.connect()) {
					onSocketStateChanged(Common.SOCKET_STATE_CONNECTING);
				}
			}
		} else if (R.id.menu_action_stop == itemid) {
			// Stop socket
			if (isConnected()) {
				if (mSocket.disconnect()) {
					mSocket = null;
				}
			}
		} else if (R.id.menu_action_keyboard == itemid) {
			// Show/Hide Keyboard
			boolean visible = kvKeyboard.getVisibility() == View.VISIBLE;
			if (visible) {
				kvKeyboard.setVisibility(View.GONE);
			} else {
				kvKeyboard.setVisibility(View.VISIBLE);
			}
		} else if (R.id.menu_action_settings == itemid) {
			// Settings
			Intent intent = new Intent(this, TPSettingsActivity.class);
			startActivity(intent);
		}
		return super.onMenuItemSelected(featureId, item);
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		
		int newOrientation = newConfig.orientation;
		if (mOldOrientation != newOrientation) {
			mOldOrientation = newOrientation;
			setContentView(R.layout.activity_main);
			findAllViews();
			createKeyboard();
		}
	}

	@Override
	public void setTextCoord(String coord) {
		tvCoord.setText(coord);
		Log.i(LOG_TAG, coord);
	}

	@Override
	public void setTextGesture(String gesture) {
		tvGesture.setText(gesture);
		Log.i(LOG_TAG, gesture);
	}

	@Override
	public void sendData(byte[] data) {
		if (!isConnected()) return;
		
		int packet_data_len = data.length + 
				Common.PACKET_CHECKSUM_SIZE +
				Common.PACKET_END_SIZE;
		byte[] head = Util.convertInt16ToBytes(Common.HEAD_FLAG);
		byte[] type = Util.convertInt16ToBytes(0);
		byte[] length = Util.convertInt32ToBytes(packet_data_len);
		byte[] checksum = Util.convertInt32ToBytes(~(Common.HEAD_FLAG+0+packet_data_len));
		byte[] end = Util.convertInt16ToBytes(Common.END_FLAG);

		int packet_len = Common.PACKET_HEAD_LENGTH + packet_data_len;
		ByteBuffer bb = ByteBuffer.allocate(packet_len);
		bb.put(head);
		bb.put(type);
		bb.put(length);
		bb.put(checksum);
		bb.put(data);
		bb.put(end);
		byte[] packet = bb.array();
		mSocket.sendData(packet);
		Log.i(LOG_TAG, Util.convertBytes2String(packet));
	}

	@Override
	public int getPointerCount(){
		return mMaxPointerCount;
	}
	
	public boolean isConnected(){
		return (mSocket != null && mSocket.isConnected());
	}

	@Override
	public KeyboardView getKeyboardView(){
		return kvKeyboard;
	}

	@Override
	public void onSocketStateChanged(int state) {
		switch(state){
		case Common.SOCKET_STATE_CONNECTING:
			Toast.makeText(this, R.string.socket_connecting, Toast.LENGTH_SHORT).show();
			break;
		case Common.SOCKET_STATE_CONNECTED:
			Toast.makeText(this, R.string.socket_connected, Toast.LENGTH_SHORT).show();
			break;
		case Common.SOCKET_STATE_DISCONNECTED:
			Toast.makeText(this, R.string.socket_disconnected, Toast.LENGTH_SHORT).show();
			break;
		case Common.SOCKET_STATE_UNKNOWN:
			break;
		}
	}

	public int ResolveAction(TPAction action)
	{
		Log.i(LOG_TAG, "ResolveAction");
		int retVal = 0;
		switch(action.action)
		{
		case Common.ACTION_DOWN:
//			retVal = OnActionDown(action);
			break;
		case Common.ACTION_UP:
//			retVal = OnActionUp(action);
			break;
		case Common.ACTION_MOVE:
//			retVal = OnActionMove(action);
			break;
		case Common.ACTION_SINGLE_TAP:
//			retVal = OnActionSingleTap(action);
			break;
		case Common.ACTION_SINGLE_TAP_UP:
//			retVal = OnActionSingleTapUp(action);
			break;
		case Common.ACTION_DOUBLE_TAP_DOWN:
//			retVal = OnActionDoubleTapDown(action);
			break;
		case Common.ACTION_DOUBLE_TAP_UP:
//			retVal = OnActionDoubleTapUp(action);
			break;
		case Common.ACTION_DOUBLE_TAP_MOVE:
//			retVal = OnActionDoubleTapMove(action);
			break;
		case Common.ACTION_FLING:
//			retVal = OnActionFling(action);
			break;
		case Common.ACTION_KK_DOWN:
			retVal = onActionKeyboardDown(action);
			break;
		case Common.ACTION_KK_UP:
			retVal = onActionKeyboardUp(action);
			break;
		case Common.ACTION_KK_PRESS:
//			retVal = OnActionKeyboardPress(action);
			break;
		case Common.ACTION_KK_TOGGLE:
			retVal = onActionKeyboardToggle(action);
			break;
		}

		return retVal;
	}

	private int onActionKeyboardDown(TPAction action){
		if (Common.ACTION_KK_DOWN != action.action) return -Common.ACTION_KK_DOWN;
		int key = action.keyboard_key;
		if (Common.KK_UNKNOWN == key) { return -Common.ACTION_KK_DOWN; }
		
		KeyboardManager.setKeyPressed(key, true);
		Log.i(LOG_TAG, "onActionKeyboardDown:"+key);
		
		return Common.ACTION_KK_DOWN;
	}

	private int onActionKeyboardUp(TPAction action){
		if (Common.ACTION_KK_UP != action.action) return -Common.ACTION_KK_UP;
		int key = action.keyboard_key;
		if (Common.KK_UNKNOWN == key) { return -Common.ACTION_KK_UP; }

		KeyboardManager.setKeyPressed(key, false);
		Log.i(LOG_TAG, "onActionKeyboardDown:"+key);
		
		return Common.ACTION_KK_UP;
	}
	private int onActionKeyboardToggle(TPAction action){
		if (Common.ACTION_KK_TOGGLE != action.action) return -Common.ACTION_KK_TOGGLE;
		int key = action.keyboard_key;
		if (Common.KK_UNKNOWN == key) { return -Common.ACTION_KK_TOGGLE; }

		boolean toggle = action.state == 1;
		KeyboardManager.setKeyStateOn(key, toggle);
		Log.i(LOG_TAG, "onActionKeyboardToggle:"+key+","+toggle);

		return Common.ACTION_KK_TOGGLE;
	}
}
