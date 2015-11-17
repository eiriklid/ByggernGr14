/*
 * oled.c
 *
 * Created: 17.09.2015 14:12:40
 *  Author: solveds
 */ 


#include "menu.h"
#include "font_5x7.h"
#include "oled.h"
#include "joystick.h" //Included for slider and button in OLED_addjust_brightness

#include <stdio.h>
#include <string.h>



static uint8_t arrow_pos = 2;
static uint8_t menu_size = 0;
static MenuNode* current_menu;

void write_c(uint8_t command){
	volatile uint8_t * oled_Adress = (volatile uint8_t*) 0x1000;
	oled_Adress[0] = command;
}

void write_d(uint8_t data){
	volatile uint8_t * oled_Adress = (volatile uint8_t*) 0x1200;
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

void OLED_print_char_inverted(char character){
	for (int i = 0; i < 5; i++)
	{
		write_d(pgm_read_word_near(&(myfont[character-32][i]) )^(0xff));
	}
}

void OLED_print_string(char* string){
	
	uint8_t length = strlen(string);
	for (int i = 0; i < length; i++)
	{
		OLED_print_char(string[i]);
	}
}

void OLED_print_string_inverted(char* string){
	
	uint8_t length = strlen(string);
	for (int i = 0; i < length; i++)
	{
		OLED_print_char_inverted(string[i]);
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

void OLED_move_arrow(uint8_t dir){
	OLED_pos(arrow_pos, 0x15);
	OLED_print_string(" ");
	arrow_pos += (dir - 2) ;
	
	if(arrow_pos < 2){
		arrow_pos = menu_size+1;
	}
	if( (arrow_pos > menu_size+1) || (menu_size == 0) ){
		arrow_pos = 2;
	}
	OLED_print_arrow(arrow_pos,0x15);
}

void OLED_pos(uint8_t row, uint8_t col){
	uint8_t low_col		= (col & (0xf) );					//And-operation between col and 0b1111
	uint8_t high_col	= ((col >> 4) & (0xf) ) + 0x10;		//Bitwise-shifting, And-Operation, Offset to write to higher column start address
	
	write_c((row & 7) + 0xb0);
	write_c(low_col);
	write_c(high_col);
}

void OLED_menu_init(){ 
	
	current_menu = menu_build();
	
	OLED_print_menu(current_menu);

	
}

void OLED_print_menu(MenuNode* node){
	//------------make headline-----------
	OLED_pos(0, 0x00);
	OLED_print_string_inverted("      ");	
	OLED_print_string_inverted(node->name);
	uint8_t string_lenght = strlen(node->name);
	
	for (uint8_t i =0; i < (20 - string_lenght) ; i++){
		OLED_print_string_inverted(" ");
	}
	
	//-----------print Raceflag------------
	OLED_pos(1,0x00);
	for (uint8_t i = 0; i < 64; i++){
		OLED_print_race_flag();		
	}
	if (node->node_func == NULL)
	{
	
	
		menu_size = node->sub_nodes;
	
		MenuNode* curr = node->child;
	
		arrow_pos = 2;
		OLED_print_arrow(arrow_pos,0x15);
	
		//----------print childs of node--------
		for (uint8_t i = 2; i < menu_size + 2 ; i++)	 
		{
			OLED_pos(i,0x20);
			OLED_print_string(curr->name);
			curr= curr->next;
		}
	}
	else{
		OLED_pos(2,0x20);
		OLED_print_string(node->description_line_1);
		
		OLED_pos(2,0x20);
		OLED_print_string(node->description_line_2);
		
		OLED_pos(4,0x20);
		OLED_print_string(node->description_line_3);
		
		OLED_pos(5,0x20);
		OLED_print_string(node->description_line_4);
	}
}

void OLED_print_submenu(){
	OLED_slide_line(arrow_pos);
	OLED_reset();	
	current_menu = menu_move_to_submenu(current_menu,arrow_pos-2); //endre til menu_move_to_sub_menu?	
	OLED_print_menu(current_menu);
	if(current_menu->node_func!= NULL){
		current_menu->node_func();
		
	}
}

void OLED_print_parentmenu(){
	
	if(current_menu->parent == NULL){
		return;
	}
	
	OLED_reset();
	current_menu = current_menu->parent;
	OLED_print_menu(current_menu);
}

void OLED_print_race_flag(){
	
	write_d(0b10101010);
	write_d(0b01010101);	
}


void OLED_slide_line(uint8_t row){
	write_c(0x27); //left scroll
	write_c(0x00); //dummy byte[A]
	write_c(row); //start page[B]
	write_c(0b111); //Time intervall between scroll step [C]
	write_c(row); //end page[D]
	write_c(0x00); //dummy byte[E]
	write_c(0xff); //dummy byte[F]
	write_c(0x2F); //Activate scroll
	
	uint8_t count = 250;
	do 
	{
		
		//printf("Sliding!"); // FINN EN BEDRE MÅTE Å ITERERE PÅ
	} while (--count);
	
	
	write_c(0x2E); //Deactivate scroll
		
	
}



