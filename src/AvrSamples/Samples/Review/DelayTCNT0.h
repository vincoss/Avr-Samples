
#ifndef DELAYTCNT0_H
#define DELAYTCNT0_H

#include <avr/io.h>
#include <avr/interrupt.h>

// Basic delay implementation with TIMER0 and Compre interrupt

volatile uint64_t millisecondTick = 0;

uint64_t DelayTCNT0_Millis(void);
void DelayTCNT0_Initialize(void);

void DelayTCNT0_Initialize(void)
{
	// Set whole register to 0
	TCCR0A = 0;

	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	
	// Set the value that you want to count to (every 1ms)
	OCR0A = 0xF9; // 249

	TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

	sei(); // enable interrupts

	// set prescaler to 64 and start the timer
	TCCR0B |= (1 << CS01) | (1 << CS00);
	
	// set timer0 counter initial value to 0
	TCNT0 = 0x0;
}

ISR(TIMER0_COMPA_vect)  // timer0 compare interrupt
{
	millisecondTick++;
}

// Returns elapsed time in milliseconds.
uint64_t DelayTCNT0_Millis()
{
	return millisecondTick;
}

#endif