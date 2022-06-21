/*
 * SHTC3.c
 *
 * Created: 07.12.2021 19:36:01
 *  Author: Rewolt
 */ 

#include "../TWI/TWI.h"
#include "SHTC3.h"

enum TWI_Status_t SHTC3_Init()
{
	return SHTC3_SendCommand(SHTC3_SLEEP);
}

enum TWI_Status_t SHTC3_SendCommand(uint16_t command)
{
	uint8_t commnad_H = (command >> 8);
	uint8_t command_L = command & 0xFF;
	return TWI_PacketTransmit(SHTC3_ADDRESS, commnad_H, &command_L, 1);
}

enum TWI_Status_t SHTC3_Measure(uint8_t *packet)
{
	SHTC3_SendCommand(SHTC3_READATA);
	return TWI_PacketReceive(SHTC3_ADDRESS, 0, packet, 6);
}