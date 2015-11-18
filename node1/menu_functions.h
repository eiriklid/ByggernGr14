/*
 * menu_functions.h
 *
 * Created: 17.11.2015 11:49:37
 *  Author: eiriklid
 */ 


#ifndef MENU_FUNCTIONS_H_
#define MENU_FUNCTIONS_H_
//These functions are used in menu.h as function pointers in MenuNode->node_func

//CAN_message id:	1 = Start game 
//					2 = Game over
//					3 = Lose life
//					4 = Controller input
//					5 = Node 2 response of starting game


void menu_func_OLED_addjust_brightness();
void menu_func_play_game();
void menu_func_highscore_reset();
void menu_func_OLED_print_highscore();
void menu_func_OLED_game_print(uint8_t lives, uint32_t time, char* game_status);

void menu_func_choose_joystick();
void menu_func_choose_slider();
void menu_func_choose_ultrasonic();



#endif /* MENU_FUNCTIONS_H_ */
