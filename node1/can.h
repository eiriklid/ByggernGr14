/*
 * can.h
 *
 * Created: 08.10.2015 09:51:00
 *  Author: solveds
 */ 


#ifndef CAN_H_
#define CAN_H_

typedef struct can_message_t{
	
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
	
	}can_message_t;


void CAN_init();
void CAN_message_send(can_message_t* can_msg);
can_message_t CAN_data_receive();




#endif /* CAN_H_ */
