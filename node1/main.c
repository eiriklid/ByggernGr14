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
	
	highscore_init();
	
	
	printf("Initialization Complete \n");
	

    while(1)
    { 
		
		
		joy_dir direction = JOY_direction(JOY_x_pos(), JOY_y_pos());
		
		if (direction == DOWN){
			OLED_move_arrow(1);
			_delay_ms(250);			
		}
		if (direction == UP){			
			OLED_move_arrow(-1);
			_delay_ms(250);
		}
		
		if (JOY_button(0)){
			OLED_print_submenu();
			_delay_ms(250);
		}
		if (JOY_button(1)){
			OLED_print_parentmenu();
			_delay_ms(250);
		}
    }
}
