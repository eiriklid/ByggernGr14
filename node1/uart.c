/*
 * uart.c
 *
 * Created: 10.09.2015 08:40:23
 *  Author: eiriklid
 */ 
#include <avr/io.h>
#include <stdint.h>

#include "uart.h"

void UART_Init(uint16_t ubrr )
{
	// Set baud rate
	UBRR0H = (uint8_t) (ubrr >>8);
	UBRR0L = (uint8_t) ubrr;
	
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
}

void UART_Transmit( uint8_t data )
{
	/* Wait for empty transmit buffer*/
	while( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data*/
	UDR0 = data;
}

uint8_t UART_Receive()
{
	/* Wait for data to be received */
	while( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer*/
	return UDR0;
}
