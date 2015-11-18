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


static uint8_t device = 1;


void menu_func_OLED_addjust_brightness(){
	while(!JOY_button(1)){
		write_c(0x81);
		write_c(JOY_slider(1));		
	}	
}

void menu_func_play_game(){
	while(JOY_direction(JOY_x_pos(), JOY_y_pos()) == NEUTRAL){
		//Waits for the player to start the game
	}
	
	can_message_t start_game_message;
	start_game_message.id = 1;
	start_game_message.length = 1;
	start_game_message.data[0] = device;
	CAN_message_send(&start_game_message);
	
	while( ((PIND & (1<< PD2)) == (1<<PD2)) ){
		//Waiting for response from node 2
	}

	can_message_t can_receive = CAN_data_receive();

	
	if (can_receive.id == 5)
	{				

		menu_func_OLED_game_print(can_receive.data[0],(uint16_t)(can_receive.data[2]<<8)+can_receive.data[1] , "Playing Game");

		while( can_receive.id != 2 ){ // Waits until Node 2 sends a game over message
		

			can_message_t joy_status;
			joy_status.id = 4;
			joy_status.length = 3;
			if (device == 1)								//Joystick
			{
				joy_status.data[0] = JOY_button(0);
				joy_status.data[1] = JOY_y_pos_raw();
				joy_status.data[2] = JOY_x_pos_raw();
				
			}
			if (device == 2)								//Slider
			{
				joy_status.data[0] = JOY_button(0);
				joy_status.data[1] = JOY_y_pos_raw();
				joy_status.data[2] = JOY_slider(1);
				
			}
			
			if (device == 3)								//Ultrasonic
			{
				joy_status.length = 2;
				joy_status.data[0] = JOY_button(0);
				joy_status.data[1] = JOY_y_pos_raw();		
			}
			
			
			CAN_message_send(&joy_status);
			
			if( !((PIND & (1<< PD2)) == (1<<PD2)) ){	//If can_msg from node 2
				can_receive = CAN_data_receive();
				if(can_receive.id == 3){
					
					menu_func_OLED_game_print(can_receive.data[0],((can_receive.data[2] << 8) + can_receive.data[1]) , "Playing Game");

				}			
			}
		}
	}
	
	menu_func_OLED_game_print(can_receive.data[0],((can_receive.data[2] << 8) + can_receive.data[1]) , "GAME OVER!");
	
	printf("GAME OVER! \n");

	uint8_t position = 0;
	position = highscore_add((can_receive.data[2] << 8) + can_receive.data[1]);
	if(position == 1){
		OLED_pos(4,0x20);
		OLED_print_string("NEW HIGHSCORE!");
	}
	
}

void menu_func_highscore_reset(){
	highscore_init();
}

void menu_func_OLED_print_highscore(){
	
	for (int i = 2; i < 7; i++) //Clears the screen, but not the header
	{
		OLED_clear_line(i);
		char buffer[17];
		OLED_pos(i,0x10);
		OLED_print_string( itoa((i-1),buffer, 10) );		
		OLED_pos(i,0x20);
		OLED_print_string( itoa(highscore_get(i-2),buffer, 10) );
	}
}

void menu_func_OLED_game_print(uint8_t lives, uint32_t time, char* game_status){

	char buffer[17];

	for (int i = 2; i < 7; i++) //Clears the screen, but not the header
	{
		OLED_clear_line(i);					
	}

	OLED_pos(2,0x50);
	OLED_print_string("Lives: ");
	OLED_print_string( itoa(lives,buffer,10) );
	OLED_pos(2,0x05);
	OLED_print_string("Time: ");
	OLED_print_string( itoa(time,buffer,10) );
	OLED_pos(6, 0x20);
	OLED_print_string( game_status );
	
}

void menu_func_choose_joystick(){
	device = 1;
}

void menu_func_choose_slider(){
	device = 2;
}
void menu_func_choose_ultrasonic(){
	device = 3;
}

