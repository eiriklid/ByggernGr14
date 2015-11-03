#include "can.h"
#include "MCP2515.h"

#include "Arduino.h"
#include "SPI.h"
#include "Servo.h"

Servo myservo;

void setup()
{
  Serial.begin(9600);
  CAN_init();
  Serial.println("Reboot"); 
  
  myservo.attach(9,900, 2100); //Maybe check if servo.attached
  myservo.write(90);    //Set servo to mid-point
  
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
  
      //**** CAN - Send
  can_message_t can_msg;
  
  can_msg.id = 1550;
  can_msg.len = 3;
  can_msg.data[0] = 250;
  can_msg.data[1] = 53;  
  can_msg.data[2] = 99;

  CAN_message_send(&can_msg);

      //****  CAN - Send - End
  
  
  delay(100);
  
      //**** CAN - Receive
  can_message_t can_receive; 
  
  can_receive= CAN_data_receive();
  
  

  Serial.print("\nID:\t ");
  Serial.print(can_receive.id);

  Serial.print("\nLenght:\t  ");
  Serial.print(can_receive.len);
  int i;
  Serial.print("\nData: \n");
  
  for(i = 0 ; i <  can_receive.len ; i++){
    Serial.print(can_receive.data[i]);
    Serial.print("\n");
    
  }
    

      //**** CAN - Receive - END
  
  /*
  Serial.print("\n\n");
  Serial.println(mcp2515_read(MCP_CANINTF), BIN);
  */

  myservo.write(can_receive.data[1]*0.7);
  
  delay(100);
 
}
