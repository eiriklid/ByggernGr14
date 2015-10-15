// 
// 
// 

#include "MCP2515.h"

#include "SPI.h"

unsigned char mcp2515_init()
{
	
	SPI.begin(); //Initialize SPI
	SPI.setClockDivider(SPI_CLOCK_DIV64);
	SPI.setDataMode(SPI_MODE0);
	pinMode(SS,OUTPUT);
	mcp2515_reset(); // Send reset-command
	mcp2515_bit_modify(MCP_RXB0CTRL, 0b01100000, 0b01100000);
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK );
	
	

	
	
	return 0;
}







unsigned char mcp2515_read(unsigned char address)
{
	
	//SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //Slave select low
	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_READ); //Send read command
	SPI.transfer(address);
	
	unsigned char i = SPI.transfer(0x00);
	
	
	
	//SPI.endTransaction();
	digitalWrite(SS,LOW);
	
	return i;
	
}

void mcp2515_write(unsigned char address, unsigned char data){

	//SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_WRITE);
	SPI.transfer(address);
	SPI.transfer(data);

	//SPI.endTransaction();
	digitalWrite(SS,HIGH);
	
	
}

void mcp2515_request_to_send(unsigned char buffer){
	
	//SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	digitalWrite(SS,LOW);
	
	if (buffer > 2)
	{
		SPI.transfer(MCP_RTS_ALL);
	}
	else
	{
		SPI.transfer( (0x80)|(1<<buffer) );
	}
	
	//SPI.endTransaction();
	
	digitalWrite(SS,HIGH);
	
	
}

unsigned char mcp2515_read_status(){
	
	//SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_READ_STATUS);
	
	unsigned char result = SPI.transfer(0x00);

	//SPI.endTransaction();
	digitalWrite(SS,HIGH);
	
	return result;
}


void mcp2515_bit_modify(unsigned char address, unsigned char mask, unsigned char data){
	
	//SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_BITMOD);
	SPI.transfer(address);
	SPI.transfer(mask);
	SPI.transfer(data);
	

	//SPI.endTransaction();
	digitalWrite(SS,HIGH);

	
}

void mcp2515_reset(){
	
	//SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_RESET);
	
	//SPI.endTransaction();
	digitalWrite(SS,LOW);
	
}


	
	
	
