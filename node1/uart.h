/*
 * uart.h
 *
 * Created: 10.09.2015 08:41:00
 *  Author: eiriklid
 */ 


#ifndef UART_H_
#define UART_H_


void UART_Init(unsigned int ubrr );

void UART_Transmit( unsigned char data );

unsigned char UART_Receive();




#endif /* UART_H_ */
