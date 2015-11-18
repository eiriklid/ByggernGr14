/*
 * spi.c
 *
 * Created: 01.10.2015 11:27:09
 *  Author: solveds
 */ 

#include <avr/io.h>
#include "MCP2515.h"
#include "macros.h"
#include "spi.h"

void SPI_MasterInit(){
	
	//Set MOSI, SCK and SS as output, MISO to input
	
	set_bit(DDRB,PB4); //SS
	set_bit(DDRB,PB5); //MOSI
	set_bit(DDRB,PB7); //SCK
	clear_bit(DDRB,PB6); //MISO
	 
	SPI_disable();
	
	set_bit(SPCR,SPE);	//Enable SPI
	set_bit(SPCR,MSTR); //Select Master
	set_bit(SPCR,SPR0); //Clock rate fck/16
	

	
}


void SPI_send(char cData){
	
	//Start transmission
	SPDR = cData;
	
	//Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));
	
	
}


char SPI_read(){
	// Send dummy data
	SPI_send(0);
	//Return data register
	return SPDR;
	
}

void SPI_enable(){

	clear_bit(PORTB,PB4); //SS low	
}

void SPI_disable(){
	
	set_bit(PORTB,PB4); //SS high
	
}
