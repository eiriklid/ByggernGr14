/*
 * joystick_sender.c
 *
 * Created: 27.10.2015 15:36:45
 *  Author: solveds
 */ 

#include <avr/io.h>

#include "joystick_sender.h"
#include "can.h"
#include "joystick.h"



void JOY_SENDER(){
	can_message_t joy_status;
	joy_status.id = 1;
	joy_status.length = 6;
	joy_status.data[0] = JOY_x_pos_raw(); 
	joy_status.data[1] = JOY_y_pos_raw();
	joy_status.data[2] = JOY_button(0);
	joy_status.data[3] = JOY_button(1);
	joy_status.data[4] = JOY_slider(0);
	joy_status.data[5] = JOY_slider(1);
	
	CAN_message_send(&joy_status);
	
}
