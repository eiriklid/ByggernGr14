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
	//struct MenuNode* (*menu_func) (struct MenuNode* node, int it); //might be unnecessary 
	void (*node_func)();
	
} MenuNode;


MenuNode* menu_init(char *name);
MenuNode* menu_insert_submenu(MenuNode *parent, char* name , void (*node_function)());
MenuNode* menu_insert_node(MenuNode* prev, MenuNode* next, char* name, void (*node_function)() );
MenuNode* menu_move_to_submenu(MenuNode* node, int it);
MenuNode* menu_build();

MenuNode* menu_run_node_func(MenuNode* node, int it);

#endif /* MENU_H_ */
