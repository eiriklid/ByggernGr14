/*
 * main.c
 *
 * Created: 10.09.2015 08:38:46
 *  Author: eiriklid
 */ 


//#define FOSC 1843200// Clock Speed
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


#include "usart.h"
#include "macros.h"
#include "sram.h"
#include "joystick.h"

int main(void)
{
	
	
	DDRB |= (1<<PB0);
	USART_Init(31);
	//unsigned char usart_char;
	MCUCR |= (1 << SRE);
	fdevopen(USART_Transmit,USART_Receive);
	//SRAM_test();
	volatile char * ADC_Adress = (char*) 0x1400;
	unsigned int x_pos_digi;
    while(1)
    {
		set_bit(PORTB,PB0);
		
		
        //Output-test ex.1
		/*
		PORTA |=(1<<PA0);
		_delay_ms(500);              
		PORTA &= ~(1<<PA0);
		_delay_ms(500);              
		*/
		
		// ex.1
		/*
		//USART_Transmit('a');
		usart_char = USART_Receive();
		USART_Transmit(usart_char);
		*/
		
		//ex.2
		/*
		clear_bit(PORTE,PE1);
		set_bit(PORTA,PA0);
		set_bit(PORTE,PE1);		
		_delay_ms(500);
		
		clear_bit(PORTE,PE1);
		set_bit(PORTA,PA1);
		set_bit(PORTE,PE1);
		_delay_ms(500);
		
		clear_bit(PORTE,PE1);
		clear_bit(PORTA,PA0);
		clear_bit(PORTA,PA1);
		set_bit(PORTE,PE1);
		_delay_ms(500);
		*/
		

		
		//printf("X er: %d \t Y er: %d \t", JOY_x_pos(),JOY_y_pos());
		
		printf("Direction: %s \n", dir2string( JOY_direction( JOY_x_pos(), JOY_y_pos()) ) );
		
		clear_bit(PORTB,PB0);
		_delay_ms(200);
    }
}
