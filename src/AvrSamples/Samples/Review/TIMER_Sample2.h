#ifndef F_CPU
#define F_CPU 20000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>

// AVR TIMER0,TIMER1,TIMER2 basic timer counter overflow and prescaler sample with counter reset

// Flash an LED every:	 8ms
// CPU clock frequency:	 16MHz
// Prescaler:			 1024

void TIMER0_Sample2_Run();

void TIMER0_Sample2_Run()
{
	// Set PB5 as output 13
	DDRB |= (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	TCCR0B = 0; // set entire register to 0, (as we do not know the initial  values) 

	// Set up timer with prescaler = 1024 (See Table 19-10. Clock Select Bit Description)
	TCCR0B |= (1 << CS02) | (1 << CS00);

	// Initialize counter / reset
	TCNT0 = 0;

	while (1)
	{
		// Read timer value and act according with it
		if (TCNT0 >= 124)  // Our pre-calculated timer count
		{
			PORTB ^= (1 << PB5);    // Toggle the led state
			TCNT0 = 0;				// Reset the timer value
		}
	}
}

void TIMER1_Sample2_Run();

void TIMER1_Sample2_Run()
{
	// Set PB5 as output 13
	DDRB |= (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	TCCR1B = 0; // set entire register to 0, (as we do not know the initial  values) 

	// Set up timer with prescaler = 1024 (Table 20-7. Clock Select Bit Description)
	TCCR1B |= (1 << CS12) | (1 << CS10);

	// Initialize counter / reset
	TCNT1 = 0;

	while (1)
	{
		// Read timer value and act according with it
		if (TCNT1 >= 124)  // Our pre-calculated timer count
		{
			PORTB ^= (1 << PB5);    // Toggle the led state
			TCNT1 = 0;				// Reset the timer value
		}
	}
}

void TIMER2_Sample2_Run();

void TIMER2_Sample2_Run()
{
	// Set PB5 as output 13
	DDRB |= (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	TCCR2B = 0; // set entire register to 0, (as we do not know the initial  values) 

	// Set up timer with prescaler = 1024 (22-10. Clock Select Bit Description)
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

	// Initialize counter / reset
	TCNT2 = 0;

	while (1)
	{
		// Read timer value and act according with it
		if (TCNT2 >= 124)  // Our pre-calculated timer count
		{
			PORTB ^= (1 << PB5);    // Toggle the led state
			TCNT2 = 0;				// Reset the timer value
		}
	}
}