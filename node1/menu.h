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
	void (*node_func)();
	char* description_line_1;
	char* description_line_2;
	char* description_line_3;
	char* description_line_4;
	
} MenuNode;


MenuNode* menu_init(char *name);
MenuNode* menu_insert_submenu(MenuNode *parent, char* name , void (*node_function)());
MenuNode* menu_insert_node(MenuNode* prev, MenuNode* next, char* name, void (*node_function)() );
MenuNode* menu_move_to_submenu(MenuNode* node, int it);
MenuNode* menu_build();

void menu_set_description_line(MenuNode* node, char* text, int line);

#endif /* MENU_H_ */
