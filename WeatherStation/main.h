#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

# define F_CPU 8000000UL
#include <avr/io.h>

void INT0_Init();
double CalculateTemp(uint8_t *packet);
double CalculateHumidity(uint8_t *packet);
void TempToString(double temperature, char *tempString, uint8_t strLength);
void HumidToString(double humidity, char *humidString, uint8_t strLength);

#endif

