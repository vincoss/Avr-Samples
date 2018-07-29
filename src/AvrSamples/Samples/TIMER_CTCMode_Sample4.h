/*
 * TIMER_CTCMode_Sample4.h
 *
 * Created: 27/07/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef TIMER_CTCMODE_SAMPLE4_H_
#define TIMER_CTCMODE_SAMPLE4_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

/*
	# AVR CTC - This mode is called Clear Timer on Compare Match, or CTC.

	Flash an LED every:		1000ms
	CPU clock frequency:	16MHz
	Prescaler:				1024
*/

volatile uint8_t _TimerCTCMode_Sample4_SensorCounter;

void TimerCTCMode_Sample4_One();

void TimerCTCMode_Sample4_One()
{
	_TimerCTCMode_Sample4_SensorCounter = 0;
	
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
		if (_TimerCTCMode_Sample4_SensorCounter == 10) // Every 10 seconds
		{
			_TimerCTCMode_Sample4_SensorCounter = 0;

			PORTB ^= (1 << PB5);    // Toggle the led state

			// Read sensor values in here
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	_TimerCTCMode_Sample4_SensorCounter++;
}

// Flash an LED every 1 second
// CPU clock frequency 2kHz
// Read sensor every 10 seconds

// Delay:			1000ms
// CPU Frequency:	16MHz
// Prescaler:		1024

// TODO: complete
void TimerCTCMode_Sample4_Two();

void TimerCTCMode_Sample4_Two()
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

// TODO: Async T2 timer

#endif