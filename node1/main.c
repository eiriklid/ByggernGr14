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
#include <avr/interrupt.h>

#include "led.h"
#include "uart.h"
#include "macros.h"
#include "sram.h"
#include "joystick.h"
#include "oled.h"
#include "MCP2515.h"
#include "spi.h"
#include "can.h"
#include "joystick_sender.h"


int main(void)
{
	// -----------Blinking Led--------------------
	led_init();
	ISR(TIMER0_OVF_vect);
	// -----------UART & printf-----------------
	UART_Init(31);
	fdevopen( UART_Transmit, UART_Receive); //Enable printf with UART
	
	// -----------SRAM-----------------
	set_bit(MCUCR,SRE); //Enable External SRAM in MCU Control Register
	
	// -----------Joystick-----------------
	JOY_init();
	
	// -----------OLED-----------------
	OLED_init();
	OLED_reset();
	OLED_menu_init();
	
	// -----------CAN-----------------
	SPI_MasterInit();	
	CAN_init();
	
	
	printf("Initialization Complete \n");
	
	
    while(1)
    { 
		
		
		
		// -----------Ex.1-----------------
		
		//printf("Exercise 1 is working properly! :D \n");
		
		
		// -----------Ex.2-----------------
		//SRAM_test();

		
		
		// -----------Ex.3-----------------
		/*	
		printf("X er: %d \t Y er: %d \n", JOY_x_pos(),JOY_y_pos());
		
		printf("Slider left: %d \t Slider right %d \n", JOY_slider(1), JOY_slider(0) );
		printf("Direction: %d \n\n", JOY_direction( JOY_x_pos(), JOY_y_pos()));
		*/
		
		
		//Menu-arrow
		
		
		joy_dir direction = JOY_direction(JOY_x_pos(), JOY_y_pos());
		
		if (direction == DOWN){
			OLED_move_arrow(3);
			_delay_ms(250);
			
		}
		if (direction == UP){			
			OLED_move_arrow(1);
			_delay_ms(250);

		}
		if (direction == RIGHT){
			OLED_print_submenu();
			_delay_ms(250);
			
		}
		if (direction == LEFT){
			OLED_print_parentmenu();
			_delay_ms(250);

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
		/*
		can_message_t can_msg;
		
		//can_msg.id = 0b11000101010;
		can_msg.id = 237;
		
		can_msg.length = 3;
		can_msg.data[0] = 24;
		can_msg.data[1] = 46;
		can_msg.data[2] = 68;
		
		
		CAN_message_send(&can_msg);
		
		
		//printf("%d \n", mcp2515_read(0x66) );
		//printf("%d \n", mcp2515_read(0x67) );
		
		*/
		if( !((PIND & (1<< PD2)) == (1<<PD2)) ){ // Venter på lav på Interrupt
		
		can_message_t can_receive = CAN_data_receive();
		
		printf("ID: %d \n", can_receive.id);
		printf("%d \n", can_receive.data[0]);
		printf("%d \n", can_receive.data[1]);
		printf("%d \n", can_receive.data[2]);
		}
		
		//JOY_SENDER();
		//printf("%d \n", JOY_button(0));
    }
}
