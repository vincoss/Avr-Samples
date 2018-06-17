/*
* main.c
*
* Created: 14/06/2018
*  Author: Ferdinand Lukasak
*/

#include <avr/io.h>
#include <util/delay.h>	


int main(int argc, char *argv[])
{
	int ledPin = 5; // pin 13

	DDRB |= (1 << ledPin); // sets the digital pin as output

	while (1)
	{
		PORTB |= (1 << ledPin); // digitalWrite HIGH
		_delay_ms(1000);

		PORTB &= ~(1 << ledPin); // digitalWrite LOW
		_delay_ms(100);
	}

	return 0;
}