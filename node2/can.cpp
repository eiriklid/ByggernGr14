// 
// 
// 

#include "stdint.h" //for unsigned char
#include "can.h"
#include "MCP2515.h"

void CAN_init(){
	
	mcp2515_init();
	
	
}

void CAN_message_send(can_message_t* can_msg){
	unsigned char low_id = ( ((can_msg->id) & (0b111))<< 5);
	unsigned char high_id = ( (can_msg->id) >> 3);
	
	
	mcp2515_write(MCP_TXB0SIDL,low_id);  //bit-modify?
	mcp2515_write(MCP_TXB0SIDH,can_msg->id);
	mcp2515_write(MCP_TXB0DLC,can_msg->length);
	
	for (unsigned char i = 0 ; i < can_msg->length ; i++)
	{
		mcp2515_write(MCP_TXB0D0 + i, can_msg->data[i] );
	}
	
	//mcp2515_request_to_send(0);
	
}

can_message_t CAN_data_receive(){
	
	
	unsigned char high_adr = mcp2515_read(MCP_TXB0SIDH);
	unsigned char low_adr = mcp2515_read(MCP_TXB0SIDL);
	
	can_message_t return_msg;
	return_msg.id = ( (high_adr<<3) | (low_adr >>5) );
	return_msg.length = mcp2515_read(MCP_TXB0DLC);
	
	for (unsigned char i = 0; i < return_msg.length; i++){
		
		return_msg.data[i] = mcp2515_read(MCP_TXB0D0 + i);
		
	}
	
	return return_msg;
	
}
