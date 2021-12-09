/*
 * SHTC3.h
 *
 * Created: 07.12.2021 19:36:21
 *  Author: Rewolt
 */ 


#ifndef SHTC3_H_
#define SHTC3_H_

enum SHTC3 
{
	SHTC3_ADDRESS	= 0x70,
	SHTC3_SLEEP		= 0xB098,
	SHTC3_WAKEUP	= 0x3517,
	SHTC3_READID	= 0xEFC8,
	SHTC3_RESET		= 0x805D,
	SHTC3_READATA	= 0x7CA2
};

void SHTC3_Init();
void SHTC3_SendCommand(uint16_t command);
void SHTC3_Measure(uint8_t *packet);

#endif /* SHTC3_H_ */