#ifndef F_CPU
#define F_CPU 16000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// CTC - This mode is called Clear Timer on Compare Match, or CTC. 
// TIMER1 - Using Interrupts with CTC Mode.
// There are three kinds of interupt: overflow, compare and capture

// Delay:			100ms
// CPU Frequency:	16MHz
// Prescaler:		64

void TimerCTCMode_Sample2_Run();

void TimerCTCMode_Sample2_Run()
{
	// Set PB5 as output 13
	DDRB = (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	cli();		// Disable global interupts

	// Set entire register to 0, (as we do not know the initial  values) 
	TCCR1A = 0;
	TCCR1B = 0;

	// initialize compare value
	OCR1A = 24999;

	// set up timer with prescaler = 64 and CTC mode
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);

	// enable compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	// enable global interrupts
	sei();

	while (1)
	{
		// do nothing
		// whenever a match occurs, ISR is fired
		// toggle the led in the ISR itself
		// no need to keep track of any flag bits here
		// done!
	}
}

// TODO: See page 157, ICP1 pin or alternatively, via the analog-comparator unit.
// TIMER1 ISR is fired whenever a match occurs hence, toggle led here itself..
ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1 << PB5);    // Toggle the led state
}