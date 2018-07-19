/*
 * TIMER_Samples_Overview.h
 *
 * Created: 20/07/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef TIMER_SAMPLES_OVERVIEW_H_
#define TIMER_SAMPLES_OVERVIEW_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

/*
	# AVR TIMER0,TIMER1,TIMER2 basic timer counter overflow sample with counter reset, no prescaling
	
	NOTE: Not runnable samples.
*/

// this code sets up a timer0 for 1ms @ 16Mhz clock cycle
// in order to function as a time delay at the begining of the main loop
// using no interrupts


void TIMER0_Sample(void)
{
	while (1)
	{
		// Set the Timer Mode to CTC
		TCCR0A |= (1 << WGM01);

		// Set the value that you want to count to
		OCR0A = 0xF9;

		// start the timer and set prescaler to 64 and start the timer
		TCCR0B |= (1 << CS01) | (1 << CS00);

		while ((TIFR0 & (1 << TOV0)) > 0)        // wait for the overflow event
		{
		}

		TIFR0 &= ~(1 << TOV0);
		// reset the overflow flag
	}
}

// this code sets up a timer0 for 4ms @ 16Mhz clock cycle
// an interrupt is triggered each time the interval occurs.

void TIMER0_COMPA_Sample(void)
{
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);

	// Set the value that you want to count to
	OCR0A = 0xF9;

	TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

	sei();         //enable interrupts

	TCCR0B |= (1 << CS02);
	// set prescaler to 256 and start the timer

	while (1)
	{
		//main loop
	}
}

ISR(TIMER0_COMPA_vect)  // timer0 overflow interrupt
{
	//event to be exicuted every 4ms here
}

// this code sets up timer1 for a 1s  @ 16Mhz Clock (mode 4)

void TIMER1_COMPA_Sample(void)
{
	OCR1A = 0x3D08; // 15624

	TCCR1B |= (1 << WGM12);
	// Mode 4, CTC on OCR1A

	TIMSK1 |= (1 << OCIE1A);
	//Set interrupt on compare match

	TCCR1B |= (1 << CS12) | (1 << CS10);
	// set prescaler to 1024 and start the timer

	sei();
	// enable interrupts


	while (1)
	{
		// we have a working Timer
	}
}

ISR(TIMER1_COMPA_vect)
{
}

// this code sets up timer1 for a 200ms  @ 16Mhz Clock (Mode 12)

void TIMER1_COMPA_Sample1(void)
{
	ICR1 = 0x30D3; // 12499

	TCCR1B |= (1 << WGM12);
	// Mode 4, CTC on OCR1A

	TIMSK1 |= (1 << ICIE1);
	//Set interrupt on compare match

	TCCR1B |= (1 << CS12);
	// set prescaler to 256 and starts the timer

	sei();
	// enable interrupts

	while (1)
	{
		// we have a working Timer
	}
}

 //this code sets up timer2 for a 250us  @ 16Mhz Clock

#include <avr/io.h>
#include <avr/interrupt.h>

void TIMER2_Sample(void)
{
	OCR2A = 62; // 3D in hex

	TCCR2A |= (1 << WGM21);
	// Set to CTC Mode

	TIMSK2 |= (1 << OCIE2A);
	//Set interrupt on compare match

	TCCR2B |= (1 << CS21);
	// set prescaler to 64 and starts PWM

	sei();
	// enable interrupts

	while (1)
	{
		// Main loop
	}
}

ISR(TIMER2_COMPA_vect)
{
	// action to be done every 250 usec
}

#endif