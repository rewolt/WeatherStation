/*
 * SSD1306.c
 *
 * Created: 06.12.2021 20:56:45
 *  Author: Rewolt
 */
 
#include "SSD13061.h"

uint8_t ch_space[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};										// Space     (32)
uint8_t ch_0[] = {248, 252, 14, 134, 134, 14, 252, 248, 63, 127, 224, 195, 195, 224, 127, 63};				//0			 (48)
uint8_t ch_1[] = {12, 12, 12, 254, 254, 0, 0, 0, 192, 192, 192, 255, 255, 192, 192, 192};					//1			 (49)
uint8_t ch_2[] = {0, 28, 12, 6, 6, 142, 254, 248, 224, 240, 248, 220, 207, 199, 195, 192};					//2			 (50)
uint8_t ch_3[] = {28, 12, 142, 134, 134, 206, 252, 120, 112, 224, 193, 193, 193, 231, 127, 60};				//3			 (51)
uint8_t ch_4[] = {128, 252, 124, 0, 0, 254, 254, 0, 15, 15, 12, 12, 12, 255, 255, 12};						//4			 (52)
uint8_t ch_5[] = {0, 254, 254, 134, 134, 134, 6, 0, 56, 121, 225, 193, 193, 227, 127, 30};					//5			 (53)
uint8_t ch_6[] = {224, 248, 156, 134, 134, 134, 6, 0, 63, 127, 225, 193, 193, 227, 63, 30};					//6			 (54)
uint8_t ch_7[] = {62, 62, 6, 6, 6, 246, 254, 14, 0, 0, 0, 240, 255, 15, 0, 0};								//7			 (55)
uint8_t ch_8[] = {56, 124, 206, 134, 134, 206, 124, 56, 62, 127, 227, 193, 193, 227, 127, 62};				//8			 (56)
uint8_t ch_9[] = {240, 252, 142, 6, 6, 14, 252, 248, 0, 193, 195, 195, 195, 115, 63, 15};					//9			 (57)
uint8_t ch_minus[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0};										// minus (-) (45)
uint8_t ch_comma[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 176, 112, 0, 0, 0};									// comma (,) (44)
uint8_t ch_C[] = {248, 252, 14, 6, 6, 14, 12, 0, 63, 127, 224, 192, 192, 224, 96, 32};						// C		 (67)
uint8_t ch_procent[] = {12, 50, 50, 12, 192, 240, 62, 14, 192, 240, 60, 15, 99, 152, 152, 96};				// percent	 (37)
uint8_t ch_degree[] = {0, 24, 60, 102, 102, 60, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0};								// degree	 (167)
uint8_t ch_line[] = {0, 0, 0, 254, 254, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0};								// |		 (179)
uint8_t ch_unknown[] = {254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255};	// full square
	


void SSD1306_Init()
{
	SSD1306_SendCommand(SSD1306_DISPLAYOFF);
	
	SSD1306_SendCommand(SSD1306_SETDISPLAYCLOCKDIV);
	SSD1306_SendCommand(0x80);
	
	SSD1306_SendCommand(SSD1306_SETMULTIPLEX);
	SSD1306_SendCommand(0x3F);
	
	SSD1306_SendCommand(SSD1306_SETDISPLAYOFFSET);
	SSD1306_SendCommand(0x0);
	
	SSD1306_SendCommand(SSD1306_SETSTARTLINE);
	
	SSD1306_SendCommand(SSD1306_CHARGEPUMP);
	SSD1306_SendCommand(0x14);
	
	SSD1306_SendCommand(SSD1306_MEMORYMODE);
	SSD1306_SendCommand(0x0);
	
	SSD1306_SendCommand(SSD1306_SEGREMAP | 0x1);
	
	SSD1306_SendCommand(SSD1306_COMSCANDEC);
	
	SSD1306_SendCommand(SSD1306_SETCOMPINS);
	SSD1306_SendCommand(0x12);
	
	SSD1306_SendCommand(SSD1306_SETCONTRAST);
	SSD1306_SendCommand(0xCF);
	
	SSD1306_SendCommand(SSD1306_SETPRECHARGE);
	SSD1306_SendCommand(0xF1);
	
	SSD1306_SendCommand(SSD1306_SETVCOMDETECT);
	SSD1306_SendCommand(0x40);
	
	SSD1306_SendCommand(SSD1306_DISPLAYALLON_RESUME);
	
	SSD1306_SendCommand(SSD1306_NORMALDISPLAY);
	
}

void SSD1306_SendCommand(uint8_t command)
{
	TWI_PacketTransmit(SSD1306_ADDRESS, 0x0, &command, 1);
}

void SSD1306_SendData(uint8_t *data, uint8_t length)
{
	TWI_PacketTransmit(SSD1306_ADDRESS, 0x40, data, length);
}

void SSD1306_ClearScreen()
{
	SSD1306_SendCommand(SSD1306_MEMORYMODE);
	SSD1306_SendCommand(0);
	SSD1306_SendCommand(SSD1306_COLUMNADDR);
	SSD1306_SendCommand(0);
	SSD1306_SendCommand(SSD1306_LCDWIDTH - 1);
	SSD1306_SendCommand(SSD1306_PAGEADDR);
	SSD1306_SendCommand(0);
	SSD1306_SendCommand(SSD1306_LCDHEIGHT - 1);
	
	uint16_t i = 1;
	uint8_t data = 0;
	while (i <= 1024)
	{
		SSD1306_SendData(&data, 1);
		i++;
	}
}
void SSD1306_PrintChar(uint8_t *character, uint8_t line, uint8_t margin)
{
	SSD1306_SendCommand(SSD1306_COLUMNADDR);
	SSD1306_SendCommand(margin);
	SSD1306_SendCommand(margin + 7);
	SSD1306_SendCommand(SSD1306_PAGEADDR);
	SSD1306_SendCommand(line);
	SSD1306_SendCommand(line + 1);
	SSD1306_SendData(character, 16);
}


void SSD1306_PrintCustomSign(uint8_t *sign, uint8_t signWidth, uint8_t startingLine, uint8_t margin)
{
	startingLine -= 1;
	SSD1306_SendCommand(SSD1306_COLUMNADDR);
	SSD1306_SendCommand(margin);
	SSD1306_SendCommand(margin + signWidth - 1);
	SSD1306_SendCommand(SSD1306_PAGEADDR);
	SSD1306_SendCommand(startingLine);
	SSD1306_SendCommand(startingLine + 1);
	SSD1306_SendData(sign, signWidth * 2);
}

/*
	Display has 8 pages (lines with 8 pixels height).
	Characters are 2 lines height (16px x 8px).
	This means we can draw characters between 1-st and 7-th line. 8-th line is reserved for bottom part of the characters addressed for 7-th line.
 */
uint8_t SSD1306_PrintString(char *string, uint8_t stringLength, uint8_t lineNumber)
{
	if(string == 0 || stringLength <= 0 || stringLength > 15 || lineNumber < 1 || lineNumber > 7)
		return 0;
	
	uint8_t margin = 1;
	uint8_t charSpace = 9;
	
	for (uint8_t i = 0; i < stringLength; i++)
	{
		char character = string[i];
		uint8_t *tableChar;
		
		if(character == 32 || character == 0)
			tableChar = ch_space;
		else if (character == 48)
			tableChar = ch_0;
		else if (character == 49)
			tableChar = ch_1;
		else if (character == 50)
			tableChar = ch_2;
		else if (character == 51)
			tableChar = ch_3;
		else if (character == 52)
			tableChar = ch_4;
		else if (character == 53)
			tableChar = ch_5;
		else if (character == 54)
			tableChar = ch_6;
		else if (character == 55)
			tableChar = ch_7;
		else if (character == 56)
			tableChar = ch_8;
		else if (character == 57)
			tableChar = ch_9;
		else if (character == 45)
			tableChar = ch_minus;
		else if (character == 44)
			tableChar = ch_comma;
		else if (character == 67)
			tableChar = ch_C;
		else if (character == 67)
			tableChar = ch_C;
		else if (character == 37)
			tableChar = ch_procent;
		else if (character == 167)
			tableChar = ch_degree;
		else if (character == 179)
			tableChar = ch_line;
		else
			tableChar = ch_unknown;
		
		SSD1306_PrintChar(tableChar, lineNumber - 1, margin);
		margin += charSpace;
	}
	
	return margin;
}