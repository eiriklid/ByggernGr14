/*
 * menu.c
 *
 * Created: 24.09.2015 13:43:57
 *  Author: eiriklid
 */ 

#include "menu.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


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
	
	return top_menu;	
}

MenuNode* menu_insert_submenu(MenuNode *parent, char* name){
	
	MenuNode *node = (MenuNode*)malloc(sizeof(MenuNode));
	
	node->name = name;
	node->next = NULL;
	node->prev = NULL;
	
	node->parent = parent;
	parent->child = node;
	parent->sub_nodes = 1;
	
	node->child = NULL;
	node->sub_nodes = 0;
	return node;
	
}

MenuNode* menu_insert_node(MenuNode* prev, MenuNode* next, char* name){
	
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
		MenuNode* play = menu_insert_submenu(main_menu,"Play Pong");
		MenuNode* h_score = menu_insert_node(play,play,"Highscore");
		MenuNode* settings = menu_insert_node(h_score,play, "Settings");
		MenuNode* excercises = menu_insert_node(h_score,settings, "Run Excercises");
		
		//--------------Exercise items-------------------------------
		MenuNode* ex_sram = menu_insert_submenu(excercises,"Exercise 2: SRAM");
		MenuNode* ex_adc = menu_insert_node(ex_sram, ex_sram, "Exercise 3: ADC");
		
		//-------------Settings items---------------------------------
		MenuNode* settings_brightness = menu_insert_submenu(settings,"Adjust Brightness");
		MenuNode* settings_reset = menu_insert_node(settings_brightness, settings_brightness, "Reset Highscore");
		
		
		return main_menu;
}
