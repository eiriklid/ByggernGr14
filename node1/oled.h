/*
 * oled.h
 *
 * Created: 17.09.2015 14:15:48
 *  Author: solveds
 */ 


#ifndef OLED_H_
#define OLED_H_
#include "menu.h"
#include <stdint.h>

void write_c(uint8_t command);
void write_d(uint8_t data);

void OLED_init();
void OLED_reset();

void OLED_print_char(char character);
void OLED_print_char_inverted(char character);

void OLED_print_string(char* string);
void OLED_print_string_inverted(char* string);

void OLED_print_arrow(uint8_t row, uint8_t col);
void OLED_pos(uint8_t row, uint8_t col);

void OLED_menu_init();    
void OLED_clear_line(uint8_t line);
void OLED_print_menu(MenuNode* node);
void OLED_move_arrow(uint8_t dir); //1 moves arrow up, 3 moves arrow down
void OLED_print_submenu();
void OLED_print_parentmenu();
void OLED_print_race_flag();
void OLED_slide_line(uint8_t row);
void OLED_addjust_brightness();

#endif /* OLED_H_ */
