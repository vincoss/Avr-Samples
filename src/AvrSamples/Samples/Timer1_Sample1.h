/*
 * Timer1_Sample1.h
 *
 * Created: 26/02/2018 7:32:52 AM
 *  Author: Ferdinand Lukasak
 */ 

/*
	Example timer with interrupt and prescaler.
	
	Tick every:				1ms
	CPU clock frequency:	100Hz
	Prescaler:				1
	Write to USART every	1 second			
*/


#ifndef TIMER1_SAMPLE1_H_
#define TIMER1_SAMPLE1_H_

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

#include <avr/interrupt.h>

volatile unsigned long long int Timer1_Sample1_Count;

void Timer1_Sample1_Main();
void Timer1_Sample1_Initialize(void);

void Timer1_Sample1_Main()
{
	Timer1_Sample1_Initialize();
	UsartInitialize();
	
	while(1)
	{
		if(Timer1_Sample1_Count >= 1000)
		{
			UsartWriteCharString("Hi AVR!");
			UsartWriteChar('\n');
			Timer1_Sample1_Count = 0;
		}
	}
}

void Timer1_Sample1_Initialize(void)
{
	// Reset ticks
	Timer1_Sample1_Count = 0;
	
	cli();	// Disable global interrupts

	TCCR1A = 0;     // set entire TCCR1A register to 0
	TCCR1B = 0;     // same for TCCR1B

	TCNT1 = 0;		// initialize counter
	OCR1A = 0x3E7F;	// Fire every 1ms, run at 1000Hz

	// Reset, waveform and prescaler
	TCCR1B &= ~(WGM12 | CS12 | CS11 | CS10);
	
	// Set waveform and prescaler to 1
	TCCR1B |= (1 << WGM12) | (1 << CS10);

	// enable compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	sei();	// enable global interrupts
}

// ISR is fired whenever a match occurs
ISR(TIMER1_COMPA_vect)
{
	Timer1_Sample1_Count++;
}

#endif /* TIMER1_SAMPLE1_H_ */