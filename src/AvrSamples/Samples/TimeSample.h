/*
 * TimeSample.h
 *
 * Created: 27/02/2018 5:40:58 PM
 *  Author: Ferdinand Lukasak
 */ 

/*
	Example time implementation with Timer1. This can be used for non blocking delay instead of delay.h implementation.
	
	Tick every:				1ms
	CPU clock frequency:	1000Hz
	Prescaler:				1
	Write to USART every	1 second			
*/

#ifndef TIME_SAMPLE_H_
#define TIME_SAMPLE_H_

#include <avr/interrupt.h>

#pragma region Timer implementation

volatile unsigned long long int TimeSample_Ticks;

void TimeSample_Initialize(void);
unsigned long long int TimeSample_GetTickCount(void);

unsigned long long int TimeSample_GetTickCount(void)
{
	return TimeSample_Ticks;
}

void TimeSample_Initialize(void)
{
	// Reset ticks
	TimeSample_Ticks = 0;
	
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
	TimeSample_Ticks++;
}

#pragma endregion

void TimeSample_Main(void)
{
	TimeSample_Initialize(); // Initialize time
	UsartInitialize();
	
	int ledPin = 5; // pin 13
	DDRB |= (1 << ledPin); // sets the digital pin as output
	
	const int milliseconds = 1000; // Second ticks
	volatile unsigned long long int ticks = TimeSample_GetTickCount(); // Get ticks count (millisecond)
	
	while(1)
	{
		if((TimeSample_GetTickCount() - ticks) >= milliseconds)
		{
			PORTB ^= (1 << ledPin); // On/Off
			
			ticks = TimeSample_GetTickCount();
			UsartWriteCharString("Hi");
		    UsartWriteChar('\n');
		}
	}
}

#endif /* TIME_SAMPLE_H_ */