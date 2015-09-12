/*
 * usart.h
 *
 * Created: 10.09.2015 08:41:00
 *  Author: eiriklid
 */ 


#ifndef USART_H_
#define USART_H_


void USART_Init(unsigned int ubrr );

void USART_Transmit( unsigned char data );

unsigned char USART_Receive();




#endif /* USART_H_ */
