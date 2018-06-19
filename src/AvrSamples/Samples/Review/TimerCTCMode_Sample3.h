#ifndef F_CPU
#define F_CPU 20000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>				
#include <avr/interrupt.h>

// TIMER2
// CTC - This mode is called Clear Timer on Compare Match, or CTC. 
// There are three kinds of interupt: overflow, compare and capture

// Delay:			1ms
// CPU Frequency:	16MHz
// Prescaler:		64

volatile uint8_t _timerCTCMode_Sample3_Event;
volatile int _timerCTCMode_Sample3_clockCompaCount;

void TimerCTCMode_Sample3_Run();

void TimerCTCMode_Sample3_Run()
{
	DDRB = (1 << PB5);		// Set PB5 as output 13
	PORTB &= ~(1 << PB5);	// Set it to low, just to be safe

	cli();		// Disable global interupts

	TCCR2B |= (1 << CS22);	// Set up prescaler = 64
	TCCR2A |= (1 << WGM21);	// Set up CTC mode

	OCR2A = 0xF9;	// initialize compare value

	TIMSK2 |= (1 << OCIE2A);	// enable compare interrupt

	_timerCTCMode_Sample3_clockCompaCount = 0;
	_timerCTCMode_Sample3_Event = 0;

	sei();	// enable global interrupts

	while (1)
	{
		if (_timerCTCMode_Sample3_Event == 1)
		{
			_timerCTCMode_Sample3_Event = 0;
			PORTB ^= (1 << PB5);    // Toggle the led state
		}
	}
}

// ISR is fired whenever a match occurs
ISR(TIMER2_COMPA_vect)
{
	_timerCTCMode_Sample3_clockCompaCount++;

	if (_timerCTCMode_Sample3_clockCompaCount >= 1000) // every one second
	{
		_timerCTCMode_Sample3_clockCompaCount = 0;	// Reset counter
		_timerCTCMode_Sample3_Event = 1;			// Set event to fire
	}
}