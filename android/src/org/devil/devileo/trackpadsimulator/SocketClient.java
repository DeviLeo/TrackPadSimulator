package org.devil.devileo.trackpadsimulator;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Vector;

import android.os.Handler;
import android.os.Handler.Callback;
import android.os.Message;
import android.util.Log;

public class SocketClient implements Runnable, Callback {
	static final String LOG_TAG = "TrackPad_SocketClient";
	
	static final int MSG_HANDLE_ACTION = 1;
	
	Socket mSocket;
	String mHost;
	int	mPort;
	Boolean mConnected;
	Handler mHandler;
	MainActivityInterface mDelegate;
	Vector<TPAction> mActions;
	
	public SocketClient(String host, int port){
		mHost = host;
		mPort = port;
		mSocket = null;
		mConnected = false;
		mHandler = new Handler(this);
		mDelegate = null;
		mActions = new Vector<TPAction>(32);
	}
	
	public boolean connect(){
		if (mConnected) {
			disconnect();
		}
		mActions.clear();
		Thread thread = new Thread(this);
		thread.start();
		return true;
	}
	
	public boolean disconnect(){
		try{
			mConnected = false;
			mSocket.close();
			mSocket = null;
			mDelegate.onSocketStateChanged(Common.SOCKET_STATE_DISCONNECTED);
		}catch(IOException ioe){
			Log.e(LOG_TAG, ioe.toString());
		}catch(Exception e){
			Log.e(LOG_TAG, e.toString());
		}
		return !mConnected;
	}
	
	public void sendData(byte[] data){
		if (mSocket == null) return;
		OutputStream os = null;
		try{
			os = mSocket.getOutputStream();
			os.write(data);
			os.flush();
		}catch(IOException ioe){
			Log.e(LOG_TAG, ioe.toString());
		}catch(Exception e){
			Log.e(LOG_TAG, e.toString());
		}
	}

	public Boolean isConnected() {
		return mConnected;
	}
	
	public void setDelegate(MainActivityInterface delegate) {
		mDelegate = delegate;
	}
	
	private void handleActions() {
		synchronized(mActions) {
			for(TPAction action : mActions) {
				mDelegate.ResolveAction(action);
			}
			mActions.clear();
		}
	}
	
	@Override
	public boolean handleMessage(Message msg) {
		switch(msg.what){
		case Common.SOCKET_STATE_CONNECTED:
			mDelegate.onSocketStateChanged(Common.SOCKET_STATE_CONNECTED);
			break;
		case MSG_HANDLE_ACTION:
			handleActions();
			break;
		}
		return false;
	}

	@Override
	public void run() {
		try{
			mSocket = new Socket(mHost, mPort);
			mConnected = true;
			mHandler.sendEmptyMessage(Common.SOCKET_STATE_CONNECTED);

			final int data_len = 2048;
			final int buf_len = 1024;
			byte data[] = new byte[data_len];	// save the rest data cannot be resolved as complete action
			byte buf[] = new byte[buf_len];		// save the received data
			int nRecv = 0;
			int nRecvZeroCount = 0;
			int nIncompleteActionIndex = 0, length = 0;
			
			InputStream is = mSocket.getInputStream();
			while (-1 != nRecv)
			{
				nRecv = is.read(buf);
				if (nRecv == 0) ++nRecvZeroCount;
				if (nRecvZeroCount > 999) break;
				if (nRecv > 0)
				{
					nRecvZeroCount = 0;

					if (length + nRecv >= data_len) { length = 0; }
//					memcpy(data+length, buf, nRecv);
					System.arraycopy(buf, 0, data, length, nRecv);
					length += nRecv;
					nIncompleteActionIndex = ResolveData(data, length);
					if (nIncompleteActionIndex <= length) {
						length -= nIncompleteActionIndex;
						if (length > 0)
						{
//							memcpy(data, data+nIncompleteActionIndex, length);
							System.arraycopy(data, nIncompleteActionIndex, data, 0, length);
						}
					}
				}
			}
			
		} catch (UnknownHostException uhe) {
			Log.e(LOG_TAG, uhe.toString());
		} catch (IOException ioe) {
			Log.e(LOG_TAG, ioe.toString());
		} catch (Exception e) {
			Log.e(LOG_TAG, e.toString());
		}
	}

	//return the last incomplete action's index
	private int ResolveData(final byte data[], final int length)
	{
		if(length <= Common.PACKET_HEAD_SIZE) return 0;

		int index = 0;

		// If the rest data length is not bigger than packet head length,
		// return, continue downloading.
		for (int i = 0; i + Common.PACKET_HEAD_LENGTH < length; ++i)
		{
			// Search Head
			int head_flag = Util.convertBytesToInt16(data, i);
			if (Common.HEAD_FLAG == head_flag)
			{
				int head_index = i;
				int packet_data_len = Util.convertBytesToInt32(data, i+Common.PACKET_HEAD_SIZE+Common.PACKET_TYPE_SIZE);
				int packet_len = Common.PACKET_HEAD_LENGTH + packet_data_len;
				int total_len = i + packet_len;

				// If the packet's end index is not bigger than data length,
				// search end flag
				if (total_len <= length)
				{
					int end_flag = Util.convertBytesToInt16(data, total_len-Common.PACKET_END_SIZE);
					// If found end flag, resolve the packet.
					if (Common.END_FLAG == end_flag)
					{
						if (0 == ResolvePacket(data, head_index, packet_len))
						{
							index = total_len;
							i = index - 1;
						}
					}
				}
				// If the packet's end index is bigger than data length,
				// break and return, continue downloading.
			}
		}

		return index;
	}

	private int ResolvePacket(final byte data[], final int offset, final int length)
	{
		if (length <= Common.PACKET_HEAD_SIZE) return -1;
		int head_flag = Util.convertBytesToInt16(data, offset);
		if (Common.HEAD_FLAG != head_flag)		return -2;
		int end_flag = Util.convertBytesToInt16(data, offset+length-Common.PACKET_END_SIZE);
		if (Common.END_FLAG != end_flag)		return -3;
		
		StringBuilder sb = new StringBuilder(128);
		for (int i = 0; i < length; i++) {
			sb.append(String.format("%02x ", data[offset+i]));
		}
		Log.i(LOG_TAG, sb.toString());
	
		int type = Util.convertBytesToInt16(data,offset+Common.PACKET_HEAD_SIZE);
		int packet_data_len = Util.convertBytesToInt32(data, offset+Common.PACKET_HEAD_SIZE+Common.PACKET_TYPE_SIZE);
		if (0 == type)
		{
			int checksum = Util.convertBytesToInt32(data, offset+8);
			if(~(head_flag+type+packet_data_len+checksum) != 0)
			{
				// Packet is broken,
				// or this is not a packet, may be data content, 
				// even if the head flag, length and end flag look like correct.
				return -4;
			}
			ResolveAction(data, offset + Common.PACKET_HEAD_LENGTH + Common.PACKET_CHECKSUM_SIZE,
				packet_data_len - Common.PACKET_CHECKSUM_SIZE - Common.PACKET_END_SIZE);
		}
		else if (1 == type)
		{
			// TODO
			//int checksum = ConvertCharToInt32(data+32);
		}
		else
		{
			// Unknown Type
		}
		return 0;
	}

	private int ResolveAction(final byte data[], final int offset, final int length)
	{
		TPAction action = new TPAction();
		action.input_type = data[offset];
		action.action = data[offset+1];
		if (Common.INPUT_TYPE_MOUSE == action.input_type)
		{
			action.pointer_count = data[offset+2];
	
			if (Common.ACTION_MOVE == action.action) 
			{
				action.distance_x = Util.convertBytesToInt32(data, offset+3);
				action.distance_y = Util.convertBytesToInt32(data, offset+7);
			}
			else if (Common.ACTION_UP == action.action)
			{
				action.distance_x = action.distance_y = 0;
			}
			else if (Common.ACTION_DOWN == action.action)
			{
				action.distance_x = action.distance_y = 0;
			}
			synchronized(mActions) {
				mActions.add(action);
			}
			mHandler.sendEmptyMessage(MSG_HANDLE_ACTION);

			String log = "Distance: " + action.distance_x + ", " + action.distance_y;
			Log.i(LOG_TAG, log);
		}
		else if (Common.INPUT_TYPE_KEYBOARD == action.input_type)
		{
			action.keyboard_key = Util.convertBytesToInt32(data, offset+2);
			if (Common.ACTION_KK_TOGGLE == action.action) {
				action.state = data[offset+6];
			}
			synchronized(mActions) {
				mActions.add(action);
			}
			mHandler.sendEmptyMessage(MSG_HANDLE_ACTION);
		}
		return 0;
	}
	
}
