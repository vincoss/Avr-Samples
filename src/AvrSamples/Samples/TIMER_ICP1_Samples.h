/*
 * TIMER_ICP1_Samples.h
 *
 * Created: 26/07/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef TIMER_ICP1_SAMPLES_H_
#define TIMER_ICP1_SAMPLES_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include "Usart.h"	
#include <stdlib.h>		
#include <util/delay.h>	

// AVR Timers – TIMER1 ICP1 sample
	
// TODO: to test this use second board to simulate pulse every second.
	
#define ICP PINB0 // ICP - Input Capture Unit (ICP1) See specification.

volatile uint16_t revTick;  // Ticks per revolution
volatile uint16_t revCtr;   // Total elapsed revolutions

void setupTimer()
{           
	TCCR1A = 0;	
	TCCR1B = 0;
	TCCR1C = 0; 

	TCCR1B |= (1 << CS12);  // Rising edge trigger, Timer = CPU Clock/256 -or- 14745600/256 -or- 57.6KHz
	
	TIMSK1 = 33;     // (00100001) Input capture and overflow interupts enabled

	TCNT1 = 0;       // start from 0
}

ISR(TIMER1_CAPT_vect)  // PULSE DETECTED!  (interrupt automatically triggered, not called by main program)
{
	revTick = ICR1;      // save duration of last revolution
	TCNT1 = 0;       // restart timer for next revolution
	revCtr++;                // add to revolution count
}

ISR(TIMER1_OVF_vect)    // counter overflow/timeout
{
	revTick = 0;
}     // RPM = 0

void IcmpSample()
{
	sei();       // enable global interrupts.
	setupTimer();    // set timer perameters
	UsartInitialize();


	PORTB |= (1 << ICP);     //pullup enabled
	DDRB &= ~(1 << ICP);     //ICR1 as input
	DDRB |= (1 << PB5);    // PB5 as output

	uint16_t RPM;    // Revolutions per minute
	char buffer[5];

	while (1)
	{  // do calculations and talk to lcd while we're not doing anything...

		if (revTick > 0) // You're not Chuck Norris, DO NOT DIVIDE BY ZERO.
		{
			RPM = 3456000 / revTick; // 3456000 ticks/minute  
		}
		else
		{
			RPM = 0;
		}

		itoa(RPM, buffer, 10); // Convert the read value to an ascii string
		UsartWriteCharString("     RPM: ");
		UsartWriteCharString(buffer);
		UsartWriteChar('\n');

		itoa(revTick, buffer, 10); // Convert the read value to an ascii string
		UsartWriteCharString("Ticks/REV: ");
		UsartWriteCharString(buffer);

		itoa(revCtr, buffer, 10); // Convert the read value to an ascii string
		UsartWriteCharString("revolutions: ");
		UsartWriteCharString(buffer);

		PORTB |= (1 << PB5);    // test signal  120RPM

		_delay_ms(499);
		PORTB &= ~(1 << PB5);

		_delay_ms(499);    // wait 1/2 a second to update... I can't read that fast...
	}

}

endif