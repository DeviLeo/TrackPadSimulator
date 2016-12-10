package org.devil.devileo.trackpadsimulator;

import android.content.Context;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;

public class GestureListener extends SimpleOnGestureListener {

	static final String LOG_TAG = "TrackPad_Gesture";
	
	Context mContext;
	MainActivityInterface mDelegate;
	
	public GestureListener(Context context, MainActivityInterface delegate) {
		super();
		mContext = context;
		mDelegate = delegate;
	}

//	@Override
//	// 双击的第二下Touch down时触发
//	public boolean onDoubleTap(MotionEvent e) {
//		String coord = e.getX() + "," + e.getY();
//		String gesture = "Double Tap, " + e.getPointerCount() + " Point(s)";
//		mDelegate.setTextCoord(coord);
//		mDelegate.setTextGesture(gesture);
//		return super.onDoubleTap(e);
//	}

	@Override
	// 双击的第二下Touch down和up都会触发，可用e.getAction()区分
	public boolean onDoubleTapEvent(MotionEvent e) {
		String coord = e.getX() + "," + e.getY();
		int action = e.getAction() & MotionEvent.ACTION_MASK;
		String strAction = null;
		byte byAction = -1;
		boolean send = false;
		if (MotionEvent.ACTION_DOWN == action){
			strAction = "Down";
			byAction = Common.ACTION_DOUBLE_TAP_DOWN;
		} else if (MotionEvent.ACTION_UP == action){
			send = true;
			strAction = "Up";
			byAction = Common.ACTION_DOUBLE_TAP_UP;
		} else if (MotionEvent.ACTION_MOVE == action){
			strAction = "Move";
			byAction = Common.ACTION_DOUBLE_TAP_MOVE;
		} else {
			strAction = String.valueOf(action);
		}

		if(send){
			byte[] data = new byte[3];
			data[0] = Common.INPUT_TYPE_MOUSE;
			data[1] = byAction;
			data[2] = (byte)mDelegate.getPointerCount();
			mDelegate.sendData(data);
		}
		
		String gesture = "Double Tap Event "+ strAction +", " + e.getPointerCount() + " Point(s)";
		mDelegate.setTextCoord(coord);
		mDelegate.setTextGesture(gesture);
		return super.onDoubleTapEvent(e);
	}

//	@Override
//	// Touch down时触发
//	public boolean onDown(MotionEvent e) {
//		String coord = e.getX() + "," + e.getY();
//		String gesture = "Down, " + e.getPointerCount() + " Point(s)";
//		mDelegate.setTextCoord(coord);
//		mDelegate.setTextGesture(gesture);
//		return super.onDown(e);
//	}

	@Override
	// Touch了滑动一点距离后，up时触发
	public boolean onFling(MotionEvent e1, MotionEvent e2,
			float velocityX, float velocityY) {
		byte[] data = new byte[3];
		data[0] = Common.INPUT_TYPE_MOUSE;
		data[1] = Common.ACTION_FLING;
		data[2] = (byte)mDelegate.getPointerCount();
		mDelegate.sendData(data);
		
		String coord = String.format("%f,%f|%f,%f|%f,%f", e1.getX(), e1.getY(), e2.getX(), e2.getY(), velocityX, velocityY);
		String gesture = String.format("Fling, %d,%d Point(s)", e1.getPointerCount(), e2.getPointerCount());
		mDelegate.setTextCoord(coord);
		mDelegate.setTextGesture(gesture);
		return super.onFling(e1, e2, velocityX, velocityY);
	}

//	@Override
//	// Touch了不移动一直Touch down时触发
//	public void onLongPress(MotionEvent e) {
//		String coord = e.getX() + "," + e.getY();
//		String gesture = "Long Press, " + e.getPointerCount() + " Point(s)";
//		mDelegate.setTextCoord(coord);
//		mDelegate.setTextGesture(gesture);
//		super.onLongPress(e);
//	}

//	@Override
//	// Touch了滑动时触发
//	public boolean onScroll(MotionEvent e1, MotionEvent e2,
//			float distanceX, float distanceY) {
//		String coord = String.format("%f,%f|%f,%f|%f,%f", e1.getX(), e1.getY(), e2.getX(), e2.getY(), distanceX, distanceY);
//		String gesture = String.format("Scroll, %d,%d Point(s)", e1.getPointerCount(), e2.getPointerCount());
//		mDelegate.setTextCoord(coord);
//		mDelegate.setTextGesture(gesture);
//		return super.onScroll(e1, e2, distanceX, distanceY);
//	}

//	@Override
//	// Touch了还没有滑动时触发
//	public void onShowPress(MotionEvent e) {
//		String coord = e.getX() + "," + e.getY();
//		String gesture = "Show Press, " + e.getPointerCount() + " Point(s)";
//		mDelegate.setTextCoord(coord);
//		mDelegate.setTextGesture(gesture);
//		super.onShowPress(e);
//	}

	@Override
	// 一根手指非常快地点击一下，onDown->onSingleTapUp->onSingleTapConfirmed
	public boolean onSingleTapConfirmed(MotionEvent e) {
		byte[] data = new byte[3];
		data[0] = Common.INPUT_TYPE_MOUSE;
		data[1] = Common.ACTION_SINGLE_TAP;
		data[2] = (byte)mDelegate.getPointerCount();
		mDelegate.sendData(data);

		String coord = e.getX() + "," + e.getY();
		String gesture = "Single Tap Confirmed, " + e.getPointerCount() + " Point(s)";
		mDelegate.setTextCoord(coord);
		mDelegate.setTextGesture(gesture);
		return super.onSingleTapConfirmed(e);
	}

	@Override
	// 多根手指按下后抬起，或者onShowPress较长时间但还没到onLongPress时间时抬起
	public boolean onSingleTapUp(MotionEvent e) {
		byte[] data = new byte[3];
		data[0] = Common.INPUT_TYPE_MOUSE;
		data[1] = Common.ACTION_SINGLE_TAP_UP;
		data[2] = (byte)mDelegate.getPointerCount();
		mDelegate.sendData(data);
		
		String coord = e.getX() + "," + e.getY();
		String gesture = "Single Tap Up, " + mDelegate.getPointerCount() + " Point(s)";
		mDelegate.setTextCoord(coord);
		mDelegate.setTextGesture(gesture);
		return super.onSingleTapUp(e);
	}

}
