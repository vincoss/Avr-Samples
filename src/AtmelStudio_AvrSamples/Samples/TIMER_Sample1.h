/*
 * TIMER_Sample1.h
 *
 * Created: 19/07/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef TIMER_SAMPLES1_H_
#define TIMER_SAMPLES1_H_

#ifndef F_CPU
	#define F_CPU 32000UL
#endif

#include <avr/io.h>

/*
	# AVR TIMER0,TIMER1,TIMER2 basic timer counter overflow sample with counter reset, no prescaling

	Flash an LED every:		6ms
	CPU clock frequency:	32kHz
	Prescaler:				1
*/

void TIMER0_Sample1_Run(void);

void TIMER0_Sample1_Run(void)
{
	// Set PB5 as output 13
	DDRB |= (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	TCCR0B = 0; // set entire register to 0, (as we do not know the initial  values) 

	// Set up timer. No prescalling (See Table 19-10. Clock Select Bit Description)
	TCCR0B |= (1 << CS00);

	// Initialize counter / reset
	TCNT0 = 0;

	while (1)
	{
		// Read timer value and act according with it
		if (TCNT0 >= 191)  // Our pre-calculated timer count
		{
			PORTB ^= (1 << PB5);    // Toggle the led state
			TCNT0 = 0;				// Reset the timer value
		}
	}
}

void TIMER1_Sample1_Run(void);

void TIMER1_Sample1_Run(void)
{
	// Set PB5 as output 13
	DDRB |= (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	TCCR1B = 0;	// set entire register to 0, (as we do not know the initial  values) 

	// Set up timer. No prescalling (Table 20-7. Clock Select Bit Description)
	TCCR1B |= (1 << CS10);

	// Initialize counter / reset
	TCNT1 = 0;

	while (1)
	{
		// Read timer value and act according with it
		if (TCNT1 >= 191)  // Our pre-calculated timer count
		{
			PORTB ^= (1 << PB5);    // Toggle the led state
			TCNT1 = 0;				// Reset the timer value
		}
	}
}

void TIMER2_Sample1_Run(void);

void TIMER2_Sample1_Run(void)
{
	// Set PB5 as output 13
	DDRB |= (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	TCCR2B = 0;	// set entire register to 0, (as we do not know the initial  values) 

	// Set up timer. No prescalling (Table 22-10. Clock Select Bit Description)
	TCCR2B |= (1 << CS20);

	// Initialize counter / reset
	TCNT2 = 0;

	while (1)
	{
		// Read timer value and act according with it
		if (TCNT2 >= 191)  // Our pre-calculated timer count
		{
			PORTB ^= (1 << PB5);    // Toggle the led state
			TCNT2 = 0;				// Reset the timer value
		}
	}
}

#endif