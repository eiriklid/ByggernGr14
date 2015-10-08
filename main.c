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
#include "MCP2515.h"
#include "spi.h"
#include "can.h"


int main(void)
{
	
	
	DDRB |= (1<<PB0);
	USART_Init(31);
	
	MCUCR |= (1 << SRE);
	fdevopen( USART_Transmit, USART_Receive);
	//SRAM_test();
	
	JOY_init();
	
	SPI_MasterInit();
	
	OLED_init();
	OLED_reset();
	
	OLED_menu();
	
	CAN_init();
	
	
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
		

		
		
		/*
		printf("X er: %d \t Y er: %d \n", JOY_x_pos(),JOY_y_pos());
		
		//printf("Slider left: %d \t Slider right %d \n", JOY_slider(1), JOY_slider(0) );
		printf("Direction: %d \n", JOY_direction( JOY_x_pos(), JOY_y_pos()));
		*/
		
		
		
		
		joy_dir direction = JOY_direction(JOY_x_pos(), JOY_y_pos());
		if (direction == DOWN){
			OLED_move_arrow(3);
			
		}
		if (direction == UP){			
			OLED_move_arrow(1);

		}
		if (direction == RIGHT){
			OLED_print_submenu();
		}
		if (direction == LEFT){
			OLED_print_parentmenu();
		}
		
		
		
		/*
		printf("Left: %d \n",JOY_button(1));
		printf("Right: %d \n",JOY_button(0));
		*/
		
		
		
		
		//Test for SPI, short circuit MISO & MOSI 
		/*
		SPI_send(0x4a);
		volatile uint8_t i = SPI_read();
		*/
		
		/*
		mcp2515_reset();
		
		mcp2515_write(0x36, 0xAA);
		
		printf("CANSTAT  0x%02x \n", mcp2515_read(0x36));
		*/
		
		//Test for CAN - send & receive msg
		can_message_t can_msg;
		
		can_msg.id = 0b11000101010;
		can_msg.length = 3;
		can_msg.data[0] = 4;
		can_msg.data[1] = 67;
		can_msg.data[2] = 83;
		
		
		CAN_message_send(&can_msg);
		
		//printf("%d \n", mcp2515_read(0x66) );
		//printf("%d \n", mcp2515_read(0x67) );
		
		can_message_t can_receive = CAN_data_receive();
		
		printf("ID: %d \n", can_receive.id);
		printf("%d \n", can_receive.data[0]);
		printf("%d \n", can_receive.data[1]);
		printf("%d \n", can_receive.data[2]);
		
		
		
		_delay_ms(50);
		clear_bit(PORTB,PB0);
		_delay_ms(50);
    }
}
