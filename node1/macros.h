/*
 * macros.h
 *
 * Created: 10.09.2015 10:50:14
 *  Author: eiriklid
 */ 
#ifndef MACROS_H_
#define MACROS_H_

#define set_bit( reg, bit) ( reg |= (1<<bit));
#define clear_bit( reg, bit) ( reg &= ~(1<<bit));
#define test_bit( reg, bit) ( reg & (1<<bit));




#endif
