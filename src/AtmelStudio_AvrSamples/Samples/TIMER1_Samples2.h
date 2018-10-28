/*
 * TIMER1_Samples2.h
 *
 * Created: 20/07/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef TIMER1_SAMPLES2_H_
#define TIMER1_SAMPLES2_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

/*
	# AVR Timers – TIMER1 with prescaler and interupts. Flash led every 1 second at frequency 1Hz

	Flash an LED every:		1 second
	CPU clock frequency:	16MHz
	Prescaler:				256
	
	NOTE:
	If you set CS10 and CS12 using: TCCR1B |= (1 << CS10); and TCCR1B |= (1 << CS12);, the clock source is divided by 1024.
	This gives a timer resolution of 1/(16*106 / 1024), or 0.000064 seconds (15625 Hz). 
	Now the timer will overflow every (65535 * 6.4*10-5s), or 4.194s.

	If you would set only CS12 using TCCR1B |=(1<<CS12); (or just TCCR1B=4), the clock source is divided by 256.
	This gives a timer resolution of 1/(16*106/256), or 0.000016 sec (62500 Hz) and the timer will overflow every (65535 *0.000016=) 1.04856 sec.

	Suppose you do not want an 1.04856 sec interval but a 1 sec interval.
	It is clear to see that if the counter wasn’t 65535 but 62500 (being equal to the frequency), the timer would be set at 1sec.
	The counter thus is 65535-62500=3035 too high. To have more precise 1 second timer we need to change only one thing – timer’s start value saved by  TCNT1 register

	(Timer Counter ).
	We do this with TCNT1=0x0BDB; BDC being the hex value of 3035. A Value of 34286 for instance would give 0.5 sec ((65535-34286)/62500)
	
*/

void TIMER1_Samples2_Run();

void TIMER1_Samples2_Run();
{
	// Set PB5 as output 13
	DDRB = (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	cli();		// Disable global interupts

	TCCR1A = 0;	// Set entire TCCR1A to 0
	TCCR1B = 0;	// Set entire TCCR1B to 0

	// Enable TIMER1 overflow interrupt
	TIMSK1 |= (1 << TOIE1);

	// Preload with value 3035
	//use 64886 for 100Hz
	//use 64286 for 50 Hz
	//use 34286 for 2 Hz
	TCNT1 = 0x0BDB; // Start count from 3035 in HEX BDB

	// Set CS12 bit so timer runs at clock speed: (with 256 prescaling)
	TCCR1B |= (1 << CS12); // Sets bit CS12 in TCCR1B
						   // This is achieved by shifting binary 1 (0b00000001)
						   // to the left by CS12 bits. This is then bitwise
						   // OR-ed into the current value of TCCR1B, which effectively set
						   // this one bit high. Similar: TCCR1B |= _BV(CS12);
						   //  or: TCCR1B= 0x04;

						   // enable global interrupts:
	sei();

	while (1)
	{
	}
}

ISR(TIMER1_OVF_vect)
{
	PORTB ^= (1 << PB5);    // Toggle the led state
	TCNT1 = 0x0BDB; // reload the timer preload from 3035
}