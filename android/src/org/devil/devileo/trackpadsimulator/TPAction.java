package org.devil.devileo.trackpadsimulator;

public class TPAction {
	public byte input_type;
	public byte action;
	public byte pointer_count;
	public int keyboard_key;
	public int distance_x;
	public int distance_y;
	public byte state;
	
	public TPAction() {
		input_type 	= 0;
		action 		= 0;
		pointer_count = 0;
		keyboard_key  = 0;
		distance_x = 0;
		distance_y = 0;
		state = 0;
	}
}