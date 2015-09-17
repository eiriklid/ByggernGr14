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

int JOY_slider(int slider){
	
	if (slider == 1)
	{
		volatile char * ADC_Adress = (char*) 0x1400;
		ADC_Adress[0]= 0x06;
		_delay_ms(1);
		return ADC_Adress[0];
		
	} 
	else
	{
		volatile char * ADC_Adress = (char*) 0x1400;
		ADC_Adress[0]= 0x07;
		_delay_ms(1);
		return ADC_Adress[0];
	}
	
}

joy_dir JOY_direction(JOY_x_pos, JOY_y_pos){

	//Neutral
	if( (-5 < JOY_x_pos() < 5) &&	(-5 < JOY_y_pos() < 5)){
		return NEUTRAL;
	}
	
	if (JOY_x_pos < JOY_y_pos){
		if(-JOY_x_pos < JOY_y_pos){
			return UP;
		}
		return LEFT;
	}
	
	else{
		if(-JOY_x_pos < JOY_y_pos){
			return RIGHT;
		}
		return DOWN;
	}

	
}


int JOY_button(int button){
	if (button ==1)
	{
		
	} 
	else
	{
		
	}
	return 0;
	
}


