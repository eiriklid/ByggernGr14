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
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL );
  uint8_t retVal = 0;
  retVal = mcp2515_read(MCP_CANCTRL);
  Serial.print("MCP CANCTRL is: ");
  Serial.println(retVal,BIN);
  
	return 0;
}

unsigned char mcp2515_read(unsigned char address)
{
	
	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_READ); //Send read command
	SPI.transfer(address);
	
	unsigned char i = SPI.transfer(0x00);

	digitalWrite(SS,HIGH);
	
	return i;
	
}

void mcp2515_write(unsigned char address, unsigned char data){

	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_WRITE);
	SPI.transfer(address);
	SPI.transfer(data);

	digitalWrite(SS,HIGH);
	
	
}

void mcp2515_request_to_send(unsigned char buf){
	
	digitalWrite(SS,LOW);
	
	if (buf > 2)
	{
		SPI.transfer(MCP_RTS_ALL);
	}
	else
	{
		SPI.transfer( (0x80)|(1<<buf) );
	}
	
	digitalWrite(SS,HIGH);
	
	
}

unsigned char mcp2515_read_status(){
	
	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_READ_STATUS);
	
	unsigned char result = SPI.transfer(0x00);

	digitalWrite(SS,HIGH);
	
	return result;
}


void mcp2515_bit_modify(unsigned char address, unsigned char mask, unsigned char data){
	
	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_BITMOD);
	SPI.transfer(address);
	SPI.transfer(mask);
	SPI.transfer(data);
	
	digitalWrite(SS,HIGH);

	
}

void mcp2515_reset(){
	
	digitalWrite(SS,LOW);
	
	SPI.transfer(MCP_RESET);
	
	digitalWrite(SS,HIGH);
	
}


	
	
	
