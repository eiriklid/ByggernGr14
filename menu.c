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


/*
typedef struct Menu {
	MenuNode * topMenu;
	
	}Menu;

Menu* menu_create()
{
	return malloc(sizeof(Menu));	
}
*/
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
	
	return top_menu;
	
	
	
	
		
}

void menu_insert_submenu(MenuNode *parent, char* name){
	
	MenuNode *node = (MenuNode*)malloc(sizeof(MenuNode));
	
	node->name = name;
	node->next = NULL;
	node->prev = NULL;
	
	node->parent = parent;
	parent->child = node;
	
	node->child = NULL;
	
}

void menu_insert_node(MenuNode* next, MenuNode* prev, char* name){
	
	MenuNode *node = (MenuNode*)malloc(sizeof(MenuNode));
	
	node->name = name;
	
	node->next = next;
	node->next->prev = node;
	
	node->prev = prev;
	node->prev->next = node;
	
	node->parent = prev->parent;
	
	node->child = NULL;
	
}



