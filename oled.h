/*
 * oled.h
 *
 * Created: 17.09.2015 14:15:48
 *  Author: solveds
 */ 


#ifndef OLED_H_
#define OLED_H_

void write_c(char command);
void write_d(char data);

void OLED_init();
void OLED_reset();
void OLED_print_char(char character);
void OLED_print_string(char* string);




#endif /* OLED_H_ */
