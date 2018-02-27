/*
 * TimeSample.h
 *
 * Created: 27/02/2018 5:40:58 PM
 *  Author: Ferdinand Lukasak
 */ 


#ifndef TIMESAMPLE_H_
#define TIMESAMPLE_H_

void TimeSample_Main()
{
	TimeSample_Initialize(); // Init time
	
	unsigned long long int ticks = TimeSample_GetTickCount(); // Get ticks count (millisecond) 
	
	while(1)
	{
		if(ticks >= 1000)
		{
			// on
		}
		else
		{
			// off
		}
	}
}

#pragma region Timer implementation

volatile unsigned long long int TimeSample_TickCount;

void TimeSample_Initialize(void);
unsigned long long int TimeSample_GetTickCount(void);

unsigned long long int TimeSample_GetTickCount(void)
{
	return TimeSample_TickCount;
}

void TimeSample_Initialize(void)
{
	// Reset ticks
	TimeSample_TickCount = 0;
	
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
	TimeSample_TickCount++;
}

#pragma endregion

#endif /* TIMESAMPLE_H_ */