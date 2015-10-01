/*
 * MCP2515.c
 *
 * Created: 01.10.2015 13:17:14
 *  Author: solveds
 */ 


#include <avr/io.h>
#include "MCP2515.h"
#include "spi.h"
#include "macros.h"


uint8_t mcp2515_read(uint8_t address){
	
	SPI_enable(); //Slave select low
	
	SPI_send(MCP_READ); //Send read command
	SPI_send(address);
	
	uint8_t i = SPI_read();
	
	
	
	SPI_disable();
	
	return i;
	
}

void mcp2515_write(uint8_t address, uint8_t data){

	SPI_enable();
	
	SPI_send(MCP_WRITE);
	SPI_send(address);
	SPI_send(data);

	SPI_disable();
	
	
}	

void mcp2515_request_to_send(){
	
	SPI_enable();
	
	SPI_send(MCP_RTS_ALL); 

	SPI_disable();
	
	
}

uint8_t mcp2515_read_status(){
	
	SPI_enable();
	
	SPI_send(MCP_READ_STATUS);
	
	uint8_t result = SPI_read();

	SPI_disable();
	
	return result;
}


void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
	
	SPI_enable();
	
	SPI_send(MCP_BITMOD);
	SPI_send(address);
	SPI_send(mask);
	SPI_send(data);
	

	SPI_disable();
	
	
}

void mcp2515_reset(){
	
	SPI_enable();
	
	SPI_send(MCP_RESET);
	
	SPI_disable();
	
}
