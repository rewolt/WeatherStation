/*
 * WeatherStation.c
 *
 * Created: 05.12.2021 18:50:04
 * Author : Rewolt
 */ 

#define TEMP_STR_LEN 7
#define HUMID_STR_LEN 6

#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "TWI/TWI.h"
#include "SSD1306/SSD13061.h"
#include "SHTC3/SHTC3.h"

	uint8_t packet[6];
	char tempString[TEMP_STR_LEN];
	char humidString[HUMID_STR_LEN];

int main(void)
{
	TWI_Setup();
	SSD1306_Init();	
	
    while (1) 
    {
		_delay_ms(1000);
		
		SHTC3_Measure(packet);
		double resultTemperature = CalculateTemp(packet);
		double resultHumidity = CalculateHumidity(packet);
		
		TempToString(resultTemperature, tempString, TEMP_STR_LEN);
		HumidToString(resultHumidity, humidString, HUMID_STR_LEN);
		
		SSD1306_PrintString(tempString, TEMP_STR_LEN, 2);
		SSD1306_PrintString(humidString, HUMID_STR_LEN, 3);
    }
}

double CalculateTemp(uint8_t *packet)
{
	uint16_t result = (uint16_t)packet[0];
	result = (result << 8);
	result = result | packet[1];
	return (double)-45 + ( (double)175 * (double)result / (double)65536 );
}

double CalculateHumidity(uint8_t *packet)
{
	uint16_t result = packet[3];
	result = (result << 8);
	result = result | packet[4];
	return (double)100 * (double)result / (double)65536;
}

void TempToString(double temperature, char *tempString, uint8_t strLength)
{
	dtostrf(temperature, strLength, 1, tempString);
	
	for (uint8_t i = 0; i < strLength; i++)
	{
		// replace "." by ","
		if (tempString[i] == 46)
		{
			tempString[i] = (char)44;
		}
		
		// left shift string by 2 chars
		if(i > 1)
		{
			tempString[i - 2] = tempString[i];
		}
	}
	
	tempString[strLength - 2] = (char)167;	// add °
	tempString[strLength - 1] = (char)67;	// add C
}

void HumidToString(double humidity, char *humidString, uint8_t strLength)
{
	dtostrf(humidity, strLength, 1, humidString);
	
	for (uint8_t i = 0; i < strLength; i++)
	{
		// replace "." by ","
		if (humidString[i] == 46)
		{
			humidString[i] = (char)44;
		}
		
		// left shift string by 1 char
		if(i > 0)
		{
			humidString[i - 1] = humidString[i];
		}
	}
	
	humidString[strLength - 1] = (char)37;	// add %
}



