#include <avr/io.h>				// This is our usual include
#include <util/delay.h>			// The delay functions and routines


void RelaySamples_SampleOne(void)
{
	uint8_t led = 5;	// Port 13

	PORTB |= (1 << led);	// Set high to keep relay off
	DDRB |= (1 << led); 

	while (1)
	{
		PORTB &= ~(1 << led); // Relay on
		
		_delay_ms(2000);

		PORTB |= (1 << led); // Relay off
		_delay_ms(2000);
	}
}