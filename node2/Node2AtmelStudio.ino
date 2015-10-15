#include "can.h"
#include "MCP2515.h"

#include "Arduino.h"
#include "SPI.h"


void setup()
{
	Serial.begin(9600);
	CAN_init();
	

}

void loop()
{
	
	/*
	SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	
	SPI.transfer(0x00);
	int j = SPI.transfer(0xAA);
	Serial.print(j,HEX);
	
	SPI.endTransaction();
	*/
	
	/*
	mcp2515_reset();
	mcp2515_write(0x36, 0x01);
	int i = mcp2515_read(0x66);
	Serial.print(i);
	*/
	
	//Test for CAN - send & receive msg
	
	
	can_message_t can_msg;
	
	can_msg.id = 1;
	can_msg.length = 2;
	can_msg.data[0] = 187;
	can_msg.data[1] = 6;
	
	CAN_message_send(&can_msg);
	delay(100);
	
		
	can_message_t can_receive = CAN_data_receive();

	Serial.print("ID: ");
	Serial.print(can_receive.id);

	Serial.print("\n");
	Serial.print(can_receive.data[0]);
	
	Serial.print("\n");
	Serial.print(can_receive.data[1]);

	Serial.print("\n");
	Serial.print(can_receive.length);
	
	Serial.print("\n\n");
	
	
	delay(100);
  
}
