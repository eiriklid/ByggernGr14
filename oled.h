/*
 * oled.h
 *
 * Created: 17.09.2015 14:15:48
 *  Author: solveds
 */ 


#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

void write_c(char command);
void write_d(char data);

void OLED_init();
void OLED_reset();
void OLED_print_char(char character);
void OLED_print_string(char* string);
void OLED_print_arrow(uint8_t row, uint8_t col);
void OLED_pos(uint8_t row, uint8_t col);
void OLED_menu();
void OLED_clear_line(uint8_t line);




#endif /* OLED_H_ */
