/*
 * joystick.c
 *
 * Created: 10.09.2015 15:13:09
 *  Author: eiriklid
 */ 

#include <avr/io.h>

#include "joystick.h"
#include "macros.h"

int digital2Prosent(int val){
	return (((val*100)/255)-50)*2;
}


uint8_t JOY_x_pos_raw(){
	volatile char * ADC_Adress = (char*) 0x1400;
	ADC_Adress[0]= 0x05;
	while( ((PIND & (1<< PD3)) == (1<<PD3))){} 
	return ADC_Adress[0];
}


uint8_t JOY_y_pos_raw(){
	volatile char * ADC_Adress = (char*) 0x1400;
	ADC_Adress[0]= 0x04;
	while( ((PIND & (1<< PD3)) == (1<<PD3))){} 
	return ADC_Adress[0];
}


int JOY_x_pos(){
	return digital2Prosent(JOY_x_pos_raw());
}


int JOY_y_pos(){
	return digital2Prosent(JOY_y_pos_raw());	
}


uint8_t JOY_slider(int slider){
	
	if (slider == 1)
	{
		volatile char * ADC_Adress = (char*) 0x1400;
		ADC_Adress[0]= 0x06;
		while( ((PIND & (1<< PD3)) == (1<<PD3))){}  
		return ADC_Adress[0];
		
	} 
	else
	{
		volatile char * ADC_Adress = (char*) 0x1400;
		ADC_Adress[0]= 0x07;
		while( ((PIND & (1<< PD3)) == (1<<PD3))){}  
		return ADC_Adress[0];
	}
	
}


joy_dir JOY_direction(int x_pos, int y_pos){
	
	if( (x_pos < 20) &&	(y_pos < 20) && (x_pos > -20) && (y_pos > -20)){
		return NEUTRAL;
	}
	
	if (x_pos < y_pos){
		if(-x_pos < y_pos){
			return UP;
		}
		return LEFT;
	}
	
	else{
		if(-x_pos < y_pos){
			return RIGHT;
		}
		return DOWN;
	}
	
}

const char* dir2string( joy_dir dir){
	
	switch(dir){
		case NEUTRAL: return "NEUTRAL";
		case LEFT:	return "LEFT";
		case RIGHT: return "RIGHT";
		case DOWN:	return "DOWN";
		case UP:	return "UP";
	}

	return 0;
}
	


int JOY_button(int button){
	if (button ==1)
	{
		return( (PINB & (1<< PB1)) == (1<<PB1));
		
	} 
	else
	{
		 return ( (PINB & (1<< PB2)) == (1<< PB2) );
	}

	return -1;
}

void JOY_init(){
	
	clear_bit(DDRB, PB1);
	clear_bit(DDRB, PB2);
	
}


