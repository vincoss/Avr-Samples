#ifndef F_CPU
#define F_CPU 20000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// CTC - This mode is called Clear Timer on Compare Match, or CTC. Pure hardware CTC

// Flash an LED every 1 second
// CPU clock frequency 2kHz
// Read sensor every 10 seconds

// Delay:			1000ms
// CPU Frequency:	16MHz
// Prescaler:		1024

// TODO: complete 
void TimerCTCMode_Sample5_Run();

void TimerCTCMode_Sample5_Run()
{
	//DDRB = (1 << PB5);		// Set PB5 as output 13
	//PORTB &= ~(1 << PB5);	// Set it to low, just to be safe

	DDRD = (1 << PB5);		// TODO: verify this

	cli();		// Disable global interupts

	TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode
	TCCR1A |= (1 << COM1A0); // Enable timer 1 Compare Output channel A in toggle mode

	OCR1A = 15624; // Set CTC compare value to 1Hz at 1 MHz AVR clock , with a prescaler of 64

	TCCR1B |= ((1 << CS10) | (1 << CS11)); // Start timer at Fcpu /64

	for (;;)
	{
	}
}