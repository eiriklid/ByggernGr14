/*
 * menu.c
 *
 * Created: 24.09.2015 13:43:57
 *  Author: eiriklid
 */ 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "menu.h"
#include "sram.h" //Included for SRAM_test
#include "menu_functions.h" //Included for OLED_addjust_brightness

MenuNode* menu_init(char *name){
	MenuNode *top_menu = (MenuNode*)malloc(sizeof(MenuNode));
	
	if (top_menu == NULL)
	{
		printf("Memory is full... \n");
		exit(1);
	}
	top_menu->name		= name;
	top_menu->parent	= NULL;
	top_menu->child		= NULL;
	top_menu->next		= NULL;
	top_menu->prev		= NULL;
	top_menu->sub_nodes	= 0;
	top_menu->node_func = NULL;
	
	return top_menu;	
}

MenuNode* menu_insert_submenu(MenuNode *parent, char* name , void (*node_function)()){
	
	MenuNode *node = (MenuNode*)malloc(sizeof(MenuNode));
	
	node->name = name;
	node->next = NULL;
	node->prev = NULL;
	
	node->parent = parent;
	parent->child = node;
	parent->sub_nodes = 1;
	
	node->child = NULL;
	node->sub_nodes = 0;
	
	node->node_func = node_function;
	
	return node;
	
}

MenuNode* menu_insert_node(MenuNode* prev, MenuNode* next, char* name, void (*node_function)() ){
	
	MenuNode *node = (MenuNode*)malloc(sizeof(MenuNode));
	
	node->name = name;
	
	node->next = next;
	node->next->prev = node;
	
	node->prev = prev;
	node->prev->next = node;
	
	node->parent = prev->parent;
	node->parent->sub_nodes += 1;
	
	node->child = NULL;
	node->sub_nodes = 0;
	
	node->node_func = node_function;
	return node;
}

MenuNode* menu_move_to_submenu(MenuNode* node, int it){
	
	MenuNode* curr = node->child;
	
	for(int i = 0; i < it; i++){
		curr = curr->next;
		
	}
	
	if(curr == NULL){
		return node;
	}
	
	return curr;
}


MenuNode* menu_build(){
		MenuNode* main_menu = menu_init("Main Menu");
		
		//--------------Main menu items------------------------------
		MenuNode* play = menu_insert_submenu(main_menu,"Play Pong",play_game);
		MenuNode* h_score = menu_insert_node(play,play,"Highscore",OLED_print_highscore);
		MenuNode* settings = menu_insert_node(h_score,play, "Settings",NULL);
		MenuNode* excercises = menu_insert_node(h_score,settings, "Run Excercises",NULL);
		
		//--------------Exercise items-------------------------------
		MenuNode* ex_sram = menu_insert_submenu(excercises,"Exercise 2: SRAM",SRAM_test);
		menu_insert_submenu(ex_sram,"Run",NULL);		
		MenuNode* ex_adc = menu_insert_node(ex_sram, ex_sram, "Exercise 3: ADC",NULL);
		
		//-------------Settings items---------------------------------
		MenuNode* settings_brightness = menu_insert_submenu(settings,"Adjust Brightness",OLED_addjust_brightness);
		menu_set_description_line(settings_brightness,"Use left slider",1);		
		MenuNode* settings_reset = menu_insert_node(settings_brightness, settings_brightness, "Reset Highscore",highscore_reset);
		
		
		return main_menu;
}



	


void menu_set_description_line(MenuNode* node, char* text, int line){
	switch (line){
		case 1:
			node->description_line_1 = text;
			break;
		case 2:
			node->description_line_2 = text;
			break;
		case 3:
			node->description_line_3 = text;
			break;
		case 4:
			node->description_line_4 = text;
			break;
		default:
			break;		
	}
}


