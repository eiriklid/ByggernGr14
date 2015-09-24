/*
 * menu.c
 *
 * Created: 24.09.2015 13:43:57
 *  Author: eiriklid
 */ 

#include "menu.h"
#include <string.h>
#include <stdlib.h>


typedef struct MenuNode {
	
	struct MenuNode *next;
	struct MenuNode *prev;
	struct MenuNode *parent;
	struct MenuNode *child;
	
	
	char* name;
	
} MenuNode;


typedef struct Menu {
	MenuNode * topMenu;
	
	}Menu;

Menu* menu_create()
{
	return malloc(sizeof(Menu));	
}

void menu_init(){
	
	
		
}

void menuNode_insert(Menu *topNode, MenuNode *next, MenuNode *prev, MenuNode *parent, MenuNode *child, char* name){
	
	MenuNode *node = (MenuNode*)malloc(sizeof(MenuNode));
	
	node->name = name;
	node->next = next;
	node->prev = prev;
	node->parent = parent;
	node->child = child;
	
}



