/*
 * macros.c
 *
 * Created: 10.09.2015 08:57:31
 *  Author: eiriklid
 */ 

#include <avr/io.h>
#include "usart.h"

set_bit( reg, bit) 
{
	( reg |= (1<<bit));
}
clear_bit( reg, bit)
{ 
	( reg &= ~(1<<bit));
}
test_bit( reg, bit)
{ 
	( reg & (1<<bit));
}
