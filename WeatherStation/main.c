/*
 * WeatherStation.c
 *
 * Created: 05.12.2021 18:50:04
 * Author : Rewolt
 */ 

#define TEMP_STR_LEN 10
#define HUMID_STR_LEN 9

#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/wdt.h>
#include "TWI/TWI.h"
#include "SSD1306/SSD13061.h"
#include "SHTC3/SHTC3.h"

uint8_t packet[6];
char tempString[TEMP_STR_LEN];
char humidString[HUMID_STR_LEN];

uint8_t thermometer[] = {0, 0, 0, 0, 0, 248, 12, 134, 134, 12, 248, 0, 0, 0, 0, 0, 0, 0, 0, 62, 99, 65, 156, 191, 191, 156, 65, 99, 62, 0, 0, 0};
uint8_t cloud_drizzle[] = {192, 32, 16, 16, 24, 4, 2, 2, 2, 4, 8, 48, 64, 64, 64, 128, 0, 97, 26, 2, 194, 50, 2, 2, 98, 26, 2, 194, 50, 2, 2, 1};
	
double counter = 0;
char counterString[TEMP_STR_LEN];

int main(void)
{
	TWI_Setup();
	SSD1306_Init();
	SHTC3_Init();
	INT0_Init();
	sei();		// enabled global interrupts
	
	
	SSD1306_SendCommand(SSD1306_DISPLAYON);
	SSD1306_PrintString("0123456789...", 13, 1);
	_delay_ms(1000);
	SSD1306_SendCommand(SSD1306_DISPLAYOFF);
	SSD1306_ClearScreen();
	
    while (1) 
    {
		// Always enter into sleep mode (power-down mode).
		// Interrupt from INT0 can wake up the device.
		MCUCR = (1 << SE) | (1 << SM1);
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
	
	tempString[strLength - 2] = (char)167;	// add ?
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

void INT0_Init()
{
	DDRD = (0 << DDRD4);	// set as input	
	PORTD = (1 << PORTD4);	// activated pull-up resistor
	GICR = (1 << INT0);		// enabled INT0 interrupt
}

ISR(INT0_vect)
{	
	SHTC3_SendCommand(SHTC3_WAKEUP);
	_delay_ms(1);
	SHTC3_Measure(packet);
	SHTC3_SendCommand(SHTC3_SLEEP);
	
	double resultTemperature = CalculateTemp(packet);
	double resultHumidity = CalculateHumidity(packet);
	
	TempToString(resultTemperature, tempString, TEMP_STR_LEN);
	HumidToString(resultHumidity, humidString, HUMID_STR_LEN);
	
	SSD1306_PrintString(tempString, TEMP_STR_LEN, 2);
	SSD1306_PrintString(humidString, HUMID_STR_LEN, 5);
	
	SSD1306_PrintCustomSign(thermometer, 16, 2, 10);
	SSD1306_PrintCustomSign(cloud_drizzle, 16, 5, 10);
	
	SSD1306_SendCommand(SSD1306_DISPLAYON);
	_delay_ms(5000);
	SSD1306_SendCommand(SSD1306_DISPLAYOFF);
}


void ShowError()
{
	SSD1306_ClearScreen();
	SSD1306_PrintString("||||||||||", TEMP_STR_LEN, 5);
	SSD1306_SendCommand(SSD1306_DISPLAYON);
	_delay_ms(5000);
	SSD1306_SendCommand(SSD1306_DISPLAYOFF);
	SSD1306_ClearScreen();
}



