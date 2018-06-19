#ifndef F_CPU
#define F_CPU 20000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// CTC - This mode is called Clear Timer on Compare Match, or CTC.

// Flash an LED every 1 second
// CPU clock frequency 2kHz
// Read sensor every 10 seconds

// Delay:			1000ms
// CPU Frequency:	16MHz
// Prescaler:		1024

volatile uint8_t _sensorCounter;

void TimerCTCMode_Sample4_Run();

void TimerCTCMode_Sample4_Run()
{
	DDRB = (1 << PB5);		// Set PB5 as output 13
	PORTB &= ~(1 << PB5);	// Set it to low, just to be safe

	cli();		// Disable global interupts

	// Set entire register to 0, (as we do not know the initial  values) 
	TCCR1A = 0;
	TCCR1B = 0;

	// Set compare timer count to fire interupt
	OCR1A = 15624;

	// turn on CTC mode:
	TCCR1B |= (1 << WGM12);

	// Set CS10 and CS12 bits for 1024 prescaler:
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);

	// enable timer compare interrupt:
	TIMSK1 |= (1 << OCIE1A);

	sei();

	while (1)
	{
		if (_sensorCounter == 10) // Every 10 seconds
		{
			_sensorCounter = 0;

			PORTB ^= (1 << PB5);    // Toggle the led state

			// Read sensor values in here
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	_sensorCounter++;
}