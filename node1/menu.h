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
	unsigned int sub_nodes;
	
} MenuNode;


MenuNode* menu_init(char *name);
MenuNode* menu_insert_submenu(MenuNode *parent, char* name);
MenuNode* menu_insert_node(MenuNode* prev, MenuNode* next, char* name);
MenuNode* menu_move_to_submenu(MenuNode* node, int it);



#endif /* MENU_H_ */
