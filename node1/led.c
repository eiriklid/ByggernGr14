/*
 * led.c
 *
 * Created: 12.11.2015 12:44:23
 *  Author: solveds
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "led.h"
#include "macros.h"

void led_init(){
	set_bit(DDRB,PB0);        //set port B bit zero to output
	TCCR0 |= (1<<CS01)|(1<<CS00);  //normal mode, no prescaling
	//ASSR  |= (1<<AS0);   //use ext oscillator
	TIMSK |= (1<<TOIE0); //allow interrupts on overflow
	
	sei();               //interrupts turned on
}


ISR(TIMER0_OVF_vect){
	static uint8_t count=0;  //hold value of count between interrupts
	count++;                 //extend counter
	//toggle PB0 each time this happens
	if((count % 64) == 0){
		PORTB ^= 0x01;
		}
}//TIMER0_OVF_vect
