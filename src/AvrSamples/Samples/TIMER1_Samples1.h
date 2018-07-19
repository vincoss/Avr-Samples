/*
 * TIMER1_Samples1.h
 *
 * Created: 20/07/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef TIMER1_SAMPLES1_H_
#define TIMER1_SAMPLES1_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

/*
	# AVR Timers – TIMER1 with prescaler and interupts. Flash led every 2 seconds at frequency 0.5Hz

	Flash an LED every:		2 seconds
	CPU clock frequency:	16MHz
	Prescaler:				8
*/

// global variable to count the number of overflows
volatile uint8_t TIMER1_Sample1_clockOverflowCount;

void TIMER1_Sample1s_Run();

void TIMER1_Sample1s_Run()
{
	// Initialize / reset overflow counter
	TIMER1_Sample1_clockOverflowCount = 0;

	// Set PB5 as output 13
	DDRB = (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	cli();  // disable global interrupts

	TCCR1B = 0;	// set entire register to 0, (as we do not know the initial  values) 

	// Set up timer with prescaler = 8
	TCCR1B |= (1 << CS11);

	// Initialize counter / reset
	TCNT1 = 0;

	// Enable overflow interrupt
	TIMSK1 |= (1 << TOIE1);

	// Enable global interrupts
	sei();

	while (1)
	{
		// check if no. of overflows
		if (TIMER1_Sample1_clockOverflowCount >= 61) // NOTE: '>=' is used
		{
			// check if the timer count reaches total timer ticks
			if (TCNT1 >= 2303)
			{
				PORTB ^= (1 << PB5);    // Toggle the led state
				TCNT1 = 0;				// Reset the timer value
				TIMER1_Sample1_clockOverflowCount = 0; // reset overflow counter. NOTE: use ATOMIC block
			}
		}
	}
}

// TIMER1 overflow interrupt service routine called whenever timer overflows
ISR(TIMER1_OVF_vect)
{
	// keep a track of number of overflows
	TIMER1_Sample1_clockOverflowCount++;
}

#endif