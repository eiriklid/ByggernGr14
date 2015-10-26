#include "can.h"
#include "MCP2515.h"

#include "Arduino.h"
#include "SPI.h"


void setup()
{
  Serial.begin(9600);
  CAN_init();
  Serial.println("Reboot"); 

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
  
  /*
  can_message_t can_msg;
  
  can_msg.id = 7;
  can_msg.len = 3;
  can_msg.data[0] = 18;
  can_msg.data[1] = 4;  
  can_msg.data[2] = 9;

   
  CAN_message_send(&can_msg);
  
*/
  
  delay(100);
  
  
  can_message_t can_receive; 
  can_receive.len = 0;
  
  
  can_receive= CAN_data_receive();
  Serial.println("Point 2");
  

  Serial.print("\nID:\t ");
  Serial.print(can_receive.id);

  Serial.print("\nLenght:\t  ");
  Serial.print(can_receive.len);
  
  Serial.print("\nData:\t");
  Serial.print(can_receive.data[0]);
  
  Serial.print("\n\t");
  Serial.print(can_receive.data[1]);
  
  Serial.print("\n\t");
  Serial.print(can_receive.data[2]);
  
  
  /*
  Serial.print("\n\n");
  Serial.println(mcp2515_read(MCP_CANINTF), BIN);
  */
  
  delay(100);
  Serial.println("\n Point 3");
  Serial.println("Point 4");
}
