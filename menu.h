/*
 * menu.h
 *
 * Created: 24.09.2015 13:44:25
 *  Author: eiriklid
 */ 


#ifndef MENU_H_
#define MENU_H_


typedef struct MenuNode {
	
	struct MenuNode *next;
	struct MenuNode *prev;
	struct MenuNode *parent;
	struct MenuNode *child;
	
	
	char* name;
	
} MenuNode;


MenuNode* menu_init(char *name);
void menu_insert_submenu(MenuNode *parent, char* name);
void menu_insert_node(MenuNode* next, MenuNode* prev, char* name);



#endif /* MENU_H_ */
