#ifndef F_CPU
#define F_CPU 20000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// CTC - This mode is called Clear Timer on Compare Match, or CTC. with overflow event

// Flash an LED every 1 second
// CPU clock frequency 2kHz
// Read sensor every 10 seconds

// Delay:			1000ms
// CPU Frequency:	16MHz
// Prescaler:		1024

volatile uint8_t _sensorCounter;

void TimerCTCMode_Sample6_Run();

void TimerCTCMode_Sample6_Run()
{
	DDRB = (1 << PB5);		// Set PB5 as output 13
	PORTB &= ~(1 << PB5);	// Set it to low, just to be safe

	cli();		// Disable global interupts

				// Set entire register to 0, (as we do not know the initial  values) 
	TCCR1A = 0;
	TCCR1B = 0;

	TIMSK1 |= (1 << TOIE1); // Enable overflow interrupt

	sei(); // Enable global interrupts
	
	TCNT1 = 49910; // Preload timer with precalculated value
	
	TCCR1B |= ((1 << CS10) | (1 << CS11)); // Set up timer at Fcpu /64

	for (;;)
	{
	}
}

ISR(TIMER1_OVF_vect)
{
	PORTB ^= (1 << PB5);    // Toggle the led state
	TCNT1 = 49910; // Reload timer with precalculated value
}