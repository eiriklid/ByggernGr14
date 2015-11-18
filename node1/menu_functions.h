/*
 * menu_functions.h
 *
 * Created: 17.11.2015 11:49:37
 *  Author: eiriklid
 */ 


#ifndef MENU_FUNCTIONS_H_
#define MENU_FUNCTIONS_H_

void menu_func_OLED_addjust_brightness();
void menu_func_play_game();
void menu_func_highscore_reset();
void menu_func_OLED_print_highscore();
void menu_func_OLED_game_print(uint8_t lives, uint16_t time, char* game_status);



#endif /* MENU_FUNCTIONS_H_ */
