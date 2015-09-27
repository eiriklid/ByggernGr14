/*
 * oled.c
 *
 * Created: 17.09.2015 14:12:40
 *  Author: solveds
 */ 


#include "menu.h"
#include "oled.h"
#include "font5x7.h"




void write_c(char command){
	volatile char * oled_Adress = (volatile char*) 0x1000;
	oled_Adress[0] = command;
}

void write_d(char data){
	volatile char * oled_Adress = (volatile char*) 0x1200;
	oled_Adress[0] = data;
}



void OLED_init()
{
	write_c(0xae);    // display off
	write_c(0xa1);    //segment remap
	write_c(0xda);    //common pads hardware: alternative
	write_c(0x12);
	write_c(0xc8);    //common output scan direction:com63~com0
	write_c(0xa8);    //multiplex ration mode:63
	write_c(0x3f);
	write_c(0xd5);    //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81);    //contrast control
	write_c(0x50);
	write_c(0xd9);    //set pre-charge period
	write_c(0x21);
	write_c(0x20);    //Set Memory Addressing Mode
	write_c(0x02);
	write_c(0xdb);    //VCOM deselect level mode
	write_c(0x30);
	write_c(0xad);    //master configuration
	write_c(0x00);
	write_c(0xa4);    //out follows RAM content
	write_c(0xa6);    //set normal display
	write_c(0xaf);    // display on
}

void OLED_reset(){
	
	for (int i = 0; i < 8; i++) //Clears the screen
	{
		OLED_clear_line(i);
	}
	
}

void OLED_clear_line(uint8_t line){
	
	OLED_pos(line, 0x00);
	for(int i = 0; i < 128; i++){
		write_d(0x00);
	}
	
}

void OLED_print_char(char character){
	
	for (int i = 0; i < 5; i++)
	{
		write_d(pgm_read_word_near(&(myfont[character-32][i])));
	}
}

void OLED_print_string(char* string){
	
	uint8_t length = strlen(string);
	for (int i = 0; i < length; i++)
	{
		OLED_print_char(string[i]);
	}
}

void OLED_print_arrow(uint8_t row, uint8_t col){
	
	OLED_pos(row, col);
	write_d(0b00011000);
	write_d(0b00011000);
	write_d(0b01111110);
	write_d(0b00111100);
	write_d(0b00011000);
	
}

void OLED_pos(uint8_t row, uint8_t col){
	uint8_t low_col		= (col & (0xf) );					//And-operation between col and 0b1111
	uint8_t high_col	= ((col >> 4) & (0xf) ) + 0x10;		//Bitwise-shifting, And-Operation, Offset to write to higher column start address
	
	write_c((row & 7) + 0xb0);
	write_c(low_col);
	write_c(high_col);
}

void OLED_menu(){
	
	MenuNode* main_menu = menu_init("Main Menu");
	
	menu_insert_submenu(main_menu,"Play Pong!");
	menu_insert_node(main_menu->child,main_menu->child,"Highscore!");
	
	
	
	OLED_pos(0, 0);
	OLED_print_string(main_menu->name);
	OLED_pos(2, 0x30);
	OLED_print_string(main_menu->child->name);
	OLED_pos(4, 0x30);
	OLED_print_string(main_menu->child->next->name);
	OLED_pos(6, 0x30);
	OLED_print_string("Settings");
	
}
