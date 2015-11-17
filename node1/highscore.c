/*
 * highscore.c
 *
 * Created: 17.11.2015 14:14:57
 *  Author: eiriklid
 */ 
#include <stdint.h>

void highscore_init(){
	volatile uint8_t *ext_ram = (uint8_t *) 0x1800; // Start address for the SRAM
	
	for (int i = 0; i < 10; i++)
	{
		ext_ram[i] = 0;
	}
}

void highscore_add(uint16_t score){
	uint16_t temp_score = 0;
	
	volatile uint8_t *ext_ram = (uint8_t *) 0x1800; // Start address for the SRAM
	
	for(int i = 0; i < 5; i++){
		temp_score = ( (ext_ram[2*i]<<8) +ext_ram[2*i+1] );
		
		if (temp_score< score){
			for (int j = 8; j >= i; j-- )
			{
				ext_ram[j+2] = ext_ram[j];  
			}
			ext_ram[2*i] = ((score>> 8)& 0xFF);
			ext_ram[2*i+1] = (score & 0xFF);
			break;				
		}
		
	}
}

uint16_t highscore_get(uint8_t index){
	volatile uint8_t *ext_ram = (uint8_t *) 0x1800; // Start address for the SRAM
	uint16_t temp_score = ( (ext_ram[2*index]<<8) + ext_ram[2*index+1] );
	return temp_score;
	
}

	
