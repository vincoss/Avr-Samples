/*
*  main.c
*
*	Created: 27/01/2018 9:43:48 PM
*	Author : Ferdinand Lukasak

	Copy main.hex into Arduino board use this command
	1. Open command prompt
	2. cd {SolutionDir}\debug
	3. avrdude -p atmega328p -c arduino -P COM6 -b 57600 -D -U flash:w:main.hex

*/

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "Utility.h"


int main(int argc, char *argv[])
{
	int ledPinSampleOne = Utility_GetSamplePin();

	DDRB |= (1 << ledPinSampleOne); // pinMode(LED, OUTPUT); // sets the gitital pin as putput

	while (1)
	{
		PORTB |= (1 << ledPinSampleOne); // digitalWrite(LED, ON); // turns the LED on
		_delay_ms(1000);

		PORTB &= ~(1 << ledPinSampleOne); // digitalWrite(LED, OFF); // turns the LED off
		_delay_ms(500);
	}

	return 0;
}
