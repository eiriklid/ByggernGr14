// can.h

#ifndef _CAN_h
#define _CAN_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

typedef struct can_message_t{
	
	unsigned int id;
	unsigned char length;
	unsigned char data[8];
	
}can_message_t;


void CAN_init();
void CAN_message_send(can_message_t* can_msg);
can_message_t CAN_data_receive();


#endif

