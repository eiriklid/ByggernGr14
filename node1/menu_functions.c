/*
 * menu_functions.c
 *
 * Created: 17.11.2015 11:49:24
 *  Author: eiriklid
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "menu_functions.h"
#include "oled.h"
#include "joystick.h"
#include "can.h"
#include "highscore.h"


void OLED_addjust_brightness(){
	while(!JOY_button(1)){
		write_c(0x81);
		write_c(JOY_slider(1));		
	}	
}

void play_game(){
	can_message_t start_game_message;
	start_game_message.id = 1;
	start_game_message.length = 1;
	start_game_message.data[0] = 0;
	CAN_message_send(&start_game_message);
	
	while( ((PIND & (1<< PD2)) == (1<<PD2)) ){ // Waits until Node 2 responds
		can_message_t joy_status;
		joy_status.id = 4;
		joy_status.length = 3;
		joy_status.data[0] = JOY_x_pos_raw();
		joy_status.data[1] = JOY_y_pos_raw();
		joy_status.data[2] = JOY_button(0);
		
		CAN_message_send(&joy_status);
		
		
	}
	can_message_t can_receive = CAN_data_receive();
	printf("GAME OVER! \n");
	printf("ID: %d \n", can_receive.id);
	printf("%d \n", can_receive.data[0]);
	printf("%d \n", can_receive.data[1]);
	printf("%d \n", can_receive.data[2]);
	
	highscore_add((can_receive.data[1] << 8) + can_receive.data[0]);
	
}

void highscore_reset(){		//Kanskje flytte til highscore.c
	highscore_init();
}

void OLED_print_highscore(){
	
	for (int i = 2; i < 7; i++) //Clears the screen, but not the header
	{
		OLED_clear_line(i);
		char buffer[16];
		OLED_pos(i,0x20);
		OLED_print_string( itoa(highscore_get(i-2),buffer, 10) );
	}
	

	
}
