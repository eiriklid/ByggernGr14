/*
 * joystick.h
 *
 * Created: 10.09.2015 15:46:49
 *  Author: eiriklid
 */ 
//Connections
/* 

JOY-CONN pin connections:
	Pin 1 (y-axis) --> ADC-CH1
	Pin 2 (x-axis) --> ADC-CH2

Slider-Filter pins:
	Pin 3 (left-slider)  --> ADC-CH3
	Pin 4 (right-slider) --> ADC-CH4

Button pins:

	Pin PD0  (left button)  --> MCU-PB1	
	Pin PB5  (right button) --> MCU-PB2

*/


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

int digital2Prosent(int val);
int JOY_x_pos();
int JOY_y_pos();
int JOY_slider(int slider);
int JOY_button(int button);

typedef enum {LEFT, RIGHT, UP, DOWN, NEUTRAL} joy_dir;


#endif /* JOYSTICK_H_ */
