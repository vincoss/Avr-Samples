#include <avr/io.h>		
#include "DelayTCNT0.h"


void DelayTCNT0Samples_Sample3(void)
{
	// Initialize delay timer
	DelayTCNT0_Initialize();

	DDRB |= (1 << PB5);
	PORTB &= ~(1 << PB5);

	unsigned long startTime = 0;

	while (1)
	{
		int delay = DelayTCNT0_Millis();

		if ((delay - startTime) >= 50)
		{
			startTime = delay;
			PORTB = PORTB ^ (1 << PB5);
		}
	}
}