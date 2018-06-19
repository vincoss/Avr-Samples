#ifndef F_CPU
#define F_CPU 20000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// AVR TIMER0 basic timer counter overflow and prescaler, interupt sample with counter reset

// Flash an LED every:	 50ms
// CPU clock frequency:	 16MHz
// Prescaler:			 256

// global variable to count the number of overflows
volatile uint8_t TIMER_Sample3_clockOverflowCount;

void TIMER0_Sample3_Run();

void TIMER0_Sample3_Run()
{
	// Set PB5 as output 13
	DDRB = (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	cli();		// Disable global interupts

	TCCR0B = 0; // set entire register to 0, (as we do not know the initial  values) 

	// Set up timer with prescaler = 256
	TCCR0B |= (1 << CS02);

	// Initialize counter / reset
	TCNT0 = 0;

	// Clear overflow flag
	TIFR0 = (1 << TOV0); // TODO: review 

	// Enable overflow interrupt
	TIMSK0 |= (1 << TOIE0);

	// Initialize / reset overflow counter
	TIMER_Sample3_clockOverflowCount = 0;

	// Enable global interrupts
	sei();

	while (1)
	{
		// check if no. of overflows = 12
		if (TIMER_Sample3_clockOverflowCount >= 12) // NOTE: '>=' is used
		{
			// check if the timer count reaches
			if (TCNT0 >= 52)
			{
				PORTB ^= (1 << PB5);    // Toggle the led state
				TCNT0 = 0;				// Reset the timer value
				TIMER_Sample3_clockOverflowCount = 0; // reset overflow counter
			}
		}
	}
}

// TIMER0 overflow interrupt service routine called whenever TCNT0 overflows
ISR(TIMER0_OVF_vect)
{
	// keep a track of number of overflows
	TIMER_Sample3_clockOverflowCount++;
}

void TIMER2_Sample3_Run();

void TIMER2_Sample3_Run()
{
	// Set PB5 as output 13
	DDRB = (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	cli();	// Disable global interupts

	TCCR2B = 0; // set entire register to 0, (as we do not know the initial  values) 

	// Set up timer with prescaler = 256
	TCCR2B |= (1 << CS22) | (1 << CS21);

	// Initialize counter / reset
	TCNT2 = 0;

	// Enable overflow interrupt
	TIMSK2 |= (1 << TOIE2);

	// Initialize / reset overflow counter
	TIMER_Sample3_clockOverflowCount = 0;

	// Enable global interrupts
	sei();

	while (1)
	{
		// check if no. of overflows = 12
		if (TIMER_Sample3_clockOverflowCount >= 12) // NOTE: '>=' is used
		{
			// check if the timer count reaches
			if (TCNT2 >= 52)
			{
				PORTB ^= (1 << PB5);    // Toggle the led state
				TCNT2 = 0;				// Reset the timer value
				TIMER_Sample3_clockOverflowCount = 0; // reset overflow counter
			}
		}
	}
}

ISR(TIMER2_OVF_vect)
{
	// keep a track of number of overflows
	TIMER_Sample3_clockOverflowCount++;
}