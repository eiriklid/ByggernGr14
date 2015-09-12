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

int JOY_button(int button){
	if (button ==1)
	{
		
	} 
	else
	{
		
	}
	return 0;
	
}


