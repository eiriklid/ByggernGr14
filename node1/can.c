/*
 * can.c
 *
 * Created: 08.10.2015 09:50:47
 *  Author: solveds
 */ 

#include <stdint.h>
#include "can.h"
#include "MCP2515.h"
#include <avr/io.h>

void CAN_init(){
	
	mcp2515_init();
}

void CAN_message_send(can_message_t* can_msg){
	
	uint8_t low_id = ( ((can_msg->id) & (0b111))<< 5); 
	uint8_t high_id = ( (can_msg->id) >> 3);
	
	mcp2515_write(MCP_TXB0SIDL,low_id);  
	mcp2515_write(MCP_TXB0SIDH,high_id);
	mcp2515_write(MCP_TXB0DLC,can_msg->length);
	
	for (uint8_t i = 0 ; i < can_msg->length ; i++)
	{
		mcp2515_write(MCP_TXB0D0 + i, can_msg->data[i] );
	}
	
	mcp2515_request_to_send(0);
}

can_message_t CAN_data_receive(){
	
	mcp2515_write( MCP_CANINTF, 0);
	
	mcp2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	uint8_t high_adr = mcp2515_read(MCP_RXB0SIDH);
	uint8_t low_adr = mcp2515_read(MCP_RXB0SIDL);
	
	can_message_t return_msg;
	return_msg.id = ( (high_adr<<3) | (low_adr >>5) );
	return_msg.length = mcp2515_read(MCP_RXB0DLC) & 0x0F;
	
	for (uint8_t i = 0; i < return_msg.length; i++){
		
		return_msg.data[i] = mcp2515_read(MCP_RXB0D0 + i);
		
		
	}
	
	return return_msg;
	
}
