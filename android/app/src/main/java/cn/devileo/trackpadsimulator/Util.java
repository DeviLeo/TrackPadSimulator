package cn.devileo.trackpadsimulator;

public class Util {
	public static byte[] convertInt32ToBytes(int i) {
		byte[] data = new byte[4];
		data[0] = (byte)(i & 0xFF);
		data[1] = (byte)((i >> 8) & 0xFF);
		data[2] = (byte)((i >> 16) & 0xFF);
		data[3] = (byte)((i >> 24) & 0xFF);
		return data;
	}
	public static int convertBytesToInt32(byte[] b) {
		return convertBytesToInt32(b, 0);
	}
	public static int convertBytesToInt32(byte[] b, int offset) {
		int i = b[offset] & 0xFF;
		i += (b[offset+1] & 0xFF) << 8;
		i += (b[offset+2] & 0xFF) << 16;
		i += (b[offset+3] & 0xFF) << 24;
		return i;
	}
	
	public static byte[] convertInt16ToBytes(int i) {
		byte[] data = new byte[2];
		data[0] = (byte)(i & 0xFF);
		data[1] = (byte)((i >> 8) & 0xFF);
		return data;
	}
	public static int convertBytesToInt16(byte[] b) {
		return convertBytesToInt16(b, 0);
	}
	public static int convertBytesToInt16(byte[] b, int offset) {
		int i = b[offset] & 0xFF;
		i += (b[offset+1] & 0xFF) << 8;
		return i;
	}
	
	public static String convertBytes2String(byte[] b) {
		int len = b.length;
		if(len==0) return "Bytes Length is ZERO!";
		StringBuilder sb = new StringBuilder(len*3);
		for(int i = 0; i < len; ++i){
			sb.append(String.format("%02x ", b[i]));
		}
		return sb.toString();
	}
}
