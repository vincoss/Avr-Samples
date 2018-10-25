/*
 * TIMER_DelayTCNT0_Sample.h
 *
 * Created: 18/07/2018 5:40:58 PM
 * Author: Ferdinand Lukasak
 */ 

/*
	Example time implementation with Timer0. This can be used for non blocking delay instead of delay.h implementation.
	
	Tick every:				1ms
	CPU clock frequency:	1000Hz
	Prescaler:				64		
*/


#ifndef TIMER_DELAYTCNT0_SAMPLE_H
#define TIMER_DELAYTCNT0_SAMPLE_H

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Usart.h"

// Basic delay implementation with TIMER0 and Compare interrupt

static volatile uint64_t _TIMER_DelayTCNT0_Sample_millisecondTick = 0;

uint64_t TIMER_DelayTCNT0_Sample_GetMillis(void);
void TIMER_DelayTCNT0_Sample_Initialize(void);

void TIMER_DelayTCNT0_Sample_Initialize(void)
{
	// Reset the counter.
	_TIMER_DelayTCNT0_Sample_millisecondTick = 0;
	
	// Set whole register to 0
	TCCR0A = 0;

	// set timer0 counter initial value to 0
	TCNT0 = 0x0;

	// Set the value that you want to count to (every 1ms)
	OCR0A = 0xF9; // 249

	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	
	// set prescaler to 64 and start the timer
	TCCR0B |= (1 << CS01) | (1 << CS00);
	
	TIMSK0 |= (1 << OCIE0A);	// Enable interrupt
	
	sei(); // enable interrupts
}

ISR(TIMER0_COMPA_vect)  // timer0 compare interrupt
{
	_TIMER_DelayTCNT0_Sample_millisecondTick++;
}

// Returns elapsed time in milliseconds.
uint64_t TIMER_DelayTCNT0_Sample_GetMillis()
{
	return _TIMER_DelayTCNT0_Sample_millisecondTick;
}

void TIMER_DelayTCNT0_Sample_Main(void)
{
	UsartInitialize(9600);
	TIMER_DelayTCNT0_Sample_Initialize();

	unsigned long startTime = 0;

	while (1)
	{
		int delay = TIMER_DelayTCNT0_Sample_GetMillis();

		if ((delay - startTime) >= 500)
		{
			startTime = delay;
			
			UsartWriteCharString("Timer0");
			UsartWriteChar('\n');
		}
	}
}

#endif