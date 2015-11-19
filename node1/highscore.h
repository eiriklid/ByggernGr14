/*
 * highscore.h
 *
 * Created: 17.11.2015 14:14:37
 *  Author: eiriklid
 */ 


#ifndef HIGHSCORE_H_
#define HIGHSCORE_H_

void highscore_init();
uint8_t highscore_add(uint16_t score);
uint16_t highscore_get(uint8_t index);




#endif /* HIGHSCORE_H_ */
