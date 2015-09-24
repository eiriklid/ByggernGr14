/*
 * main.c
 *
 * Created: 10.09.2015 08:38:46
 *  Author: eiriklid
 */ 


//#define FOSC 1843200// Clock Speed
#define F_CPU 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


#include "usart.h"
#include "macros.h"
#include "sram.h"
#include "joystick.h"
#include "oled.h"

int main(void)
{
	
	
	DDRB |= (1<<PB0);
	USART_Init(31);
	
	MCUCR |= (1 << SRE);
	fdevopen(USART_Transmit,USART_Receive);
	//SRAM_test();
	
	JOY_init();
	
	OLED_init();
	OLED_reset();
	
	int pageCom = 0xb0;

	
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
		//printf("Slider left: %d \t Slider right %d \n", JOY_slider(1), JOY_slider(0) );
		//printf("Direction: %d \n", JOY_direction( JOY_x_pos(), JOY_y_pos()));
		
		
		
		/*
		write_c(pageCom++);
		write_c(0x0a);
		write_c(0x10);
		OLED_print_string("lol, er det mulig?");
		if (pageCom == 0xb7)
		{pageCom=0xb0;
		}
		*/
		
		OLED_menu();
		
		OLED_print_arrow(2, 0x20);
		
		joy_dir direction = JOY_direction(JOY_x_pos(), JOY_y_pos());
		if (direction == DOWN){
			OLED_print_arrow(4, 0x20);
				
		}
		if (direction == UP){
			OLED_print_arrow(6, 0x20);
		}
		
		
		printf("Left: %d \n",JOY_button(1));
		printf("Right: %d \n",JOY_button(0));

		
		_delay_ms(200);
		clear_bit(PORTB,PB0);
		_delay_ms(200);
    }
}
