/*
 * joystick.c
 *
 * Created: 10.09.2015 15:13:09
 *  Author: eiriklid
 */ 

#include <util/delay.h>

#include "joystick.h"

int digital2Prosent(int val){
	return (((val*100)/255)-50)*2;
}

//bool JOY_button(int button);

int JOY_x_pos(){
	volatile char * ADC_Adress = (char*) 0x1400;
	ADC_Adress[0]= 0x05;
	_delay_ms(1); 
	return digital2Prosent(ADC_Adress[0]);
}


int JOY_y_pos(){
	volatile char * ADC_Adress = (char*) 0x1400;
	ADC_Adress[0]= 0x04;
	_delay_ms(1);
	return digital2Prosent(ADC_Adress[0]);
	
}
