/*
 * spi.h
 *
 * Created: 01.10.2015 11:26:57
 *  Author: solveds
 */ 


#ifndef SPI_H_
#define SPI_H_



void SPI_MasterInit();
void SPI_send(char cData);
char SPI_read();
void SPI_enable();
void SPI_disable();


#endif /* SPI_H_ */
