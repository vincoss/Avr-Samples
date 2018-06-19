#ifndef F_CPU
#define F_CPU 20000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  

// CTC - This mode is called Clear Timer on Compare Match, or CTC. 
// TIMER1 - Clear Timer on Compare (CTC) Mode.

// Delay:			100ms
// CPU Frequency:	16MHz
// Prescaler:		64

void TimerCTCMode_Sample1_Run(void);

void TimerCTCMode_Sample1_Run(void)
{
	// Set PB5 as output 13
	DDRB = (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	// Set entire register to 0, (as we do not know the initial  values) 
	TCCR1A = 0; 
	TCCR1B = 0;

	// initialize compare value
	OCR1A = 24999;

	// Set up timer with prescaler = 64 and CTC mode
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);

	while (1)
	{
		// check whether the flag bit is set
		// if set, it means that there has been a compare match
		// and the timer has been cleared
		// use this opportunity to toggle the led
		if (TIFR1 & (1 << OCF1A)) // NOTE: '>=' used instead of '=='
		{
			PORTB ^= (1 << PB5);    // Toggle the led state

			// wait! we are not done yet!
			// clear the flag bit manually since there is no ISR to execute
			// clear it by writing '1' to it (as per the datasheet)
			TIFR1 |= (1 << OCF1A);
		}
	}
}

#define green_led_on()  PORTC |= _BV(0)
#define green_led_off()  PORTC &= ~_BV(0)
#define green_led_is_on() bit_is_set(PORTC,0)

void TimerCTCMode_SampleTwo_Run(void);

void TimerCTCMode_SampleTwo_Run(void)
{
	DDRC = 0b01111111;   // All outputs (Although we will just use PC0 )

	TIMSK1 = _BV(OCIE1A);  // Enable Interrupt TimerCounter0 Compare Match A (TIMER1_COMPA_vect)
	TCCR1A = _BV(WGM12);  // Mode = CTC
	TCCR1B = _BV(CS10) | _BV(CS12);   // Clock/1024
	OCR1A = 244;

	sei();

	while (1)
	{
	}
}

//ISR(TIMER1_COMPA_vect)
//{
//	if (green_led_is_on())
//	{
//		green_led_off();
//	}
//	else
//	{
//		green_led_on();
//	}
//}

#define green_led_on()  PORTC |= _BV(0)
#define green_led_off()  PORTC &= ~_BV(0)

int TimerCTCMode_SampleThree_Run(void);

int TimerCTCMode_SampleThree_Run(void)
{
	DDRC = 0b01111111;   // All outputs (Although we will just use PC0 )

	TIMSK1 = _BV(OCIE1A) | _BV(OCIE1B);  // Enable Interrupt TimerCounter0 Compare Match A & B
	TCCR1A = _BV(WGM12);               // Mode = CTC
	TCCR1B = _BV(CS10) | _BV(CS12);     // Clock/1024
	OCR1A = 244;
	OCR1B = 22220;

	sei();

	while (1)
	{
	}
}

ISR(TIMER1_COMPA_vect)
{
	green_led_off();
}

ISR(TIMER1_COMPB_vect)
{
	green_led_on();
}