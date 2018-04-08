#include <avr/io.h>
#include <avr/delay.h>
#include "Utility.h"


int main(int argc, char *argv[])
{
	/*uint8_t binary_counter = 0;
	uint8_t shift = binary_counter >> 8;

	printf("%d %d", binary_counter, shift);*/

	//int ledPinSampleOne = Utility_GetSamplePin();

	//DDRB |= (1 << ledPinSampleOne); // pinMode(LED, OUTPUT); // sets the gitital pin as putput



	//while (1)
	//{
	//	PORTB |= (1 << ledPinSampleOne); // digitalWrite(LED, ON); // turns the LED on
	//	_delay_ms(100);

	//	PORTB &= ~(1 << ledPinSampleOne); // digitalWrite(LED, OFF); // turns the LED off
	//	_delay_ms(500);
	//}

	return 0;
}
