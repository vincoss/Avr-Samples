#ifndef F_CPU
#define F_CPU 20000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>				// This is our usual include
#include <avr/interrupt.h>
#include <stdlib.h>

// On an Uno, analogWrite() works only with pins 3, 5, 6, 9, 10, and 11,

// We need to generate a 50 Hz PWM signal having 45% duty cycle.
// On:					9ms
// Off:					11ms
// CPU clock frequency:	32kHz
// Prescaler:			1024

volatile uint8_t count = 0; // global counter

// initialize timer, interrupt and variable
void Pwm_Samples1_Sample1()
{
	
	// set up timerX with suitable prescaler and CTC mode
	// initialize counter
	// initialize compare value
	// enable compare interrupt
	// enable global interrupts

	cli();		// Disable global interupts

	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);

	// Set up timer with prescaler = 1024
	TCCR0B (1 << CS02) | (1 << CS00);

	// initialize counter
	TCNT0 = 0;

	// initialize compare value
	OCR1A = 139;

	// enable compare interrupt
	TIMSK0 |= (1 << OCIE0A);

	// enable global interrupts
	sei();

	while (1)
	{
		//main loop
	}
}

ISR(TIMER0_COMPA_vect)
{
	count = 0;
}


void BlinkingLedSamples_PwmFadeLed()
{
	//// Initialize
	//TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
	//OCR0A = 128;

	////TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);      // Non inverting mode on OC0A and OC0B, Mode = Mode 3 FAST PWM
	////TCCR0B = _BV(CS00);                                                // No prescaling

	////TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);      // Non inverting mode on OC2A, Mode = Mode 3 FAST PWM
	////TCCR2B = _BV(CS20);
	//
	//// connect pwm to pin on timer 0, channel B
	////sbi(TCCR0A, COM0B1);
	////OCR0B = val; // set pwm duty

	//int8_t pin = PB3;// 9
	//
	//DDRB |= (1 << pin); // output

	//while (1)
	//{
	//	for (int i = 0; i < 255; i++) // Fade in
	//	{
	//		OCR0A = i;  // set the LED brightness 
	//		_delay_ms(10);
	//	}

	//	for (int i = 255; i > 0; i--) // Fade out
	//	{
	//		OCR0A = i;  // set the LED brightness
	//		_delay_ms(10);
	//	}
	//}

	//// 2

	//TIMSK0 |= 1 << OCIE0A;
	//TCCR0A |= 1 << WGM00 | 1 << WGM01;
	//TCCR0B |= 1 << WGM02;
	//OCR0A = 124;
	////sei();
	//TCCR0B |= 1 << CS02 | 1 << CS00;
	//while (1)
	//{
	//		for (int i = 0; i < 255; i++) // Fade in
	//		{
	//			OCR0A = i;  // set the LED brightness 
	//			_delay_ms(10);
	//		}

	//		for (int i = 255; i > 0; i--) // Fade out
	//		{
	//			OCR0A = i;  // set the LED brightness
	//			_delay_ms(10);
	//		}
	//}

	////// 3

	// Waveform Generation Mode - Fast PWM
	TCCR0A |= (1 << WGM00) | (1 << WGM01);

	// Compare Match Output Mode 
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0);

	// Set prescaler to (no prescaling)
	TCCR0B |= (1 << CS00);

	// make sure to make OC0 pin (pin PB3 for atmega32) as output pin
	/*DDRB |= (1 << PB3);
	PORTB &= ~(1 << PB3);*/

	// Make PD6 port output and make it low. OCR0A uses PD6 port
	DDRD |= (1 << PD6);
	PORTD &= ~(1 << PD6);

	// run forever
	while (1)
	{
		// increasing brightness
		for (int brightness = 0; brightness < 255; ++brightness)
		{
			// set the brightness as duty cycle
			OCR0A = brightness;

			// delay so as to make the user "see" the change in brightness
			_delay_ms(10);
		}

		// decreasing brightness
		for (int brightness = 255; brightness > 0; --brightness)
		{
			// set the brightness as duty cycle
			OCR0A = brightness;

			// delay so as to make the user "see" the change in brightness
			_delay_ms(10);
		}
	}

	///// 4

	//DDRB |= (1 << PB2);
	//PORTB &= ~(1 << PB2);

	//TCCR0A = (1 << COM0A1) | (1 << WGM00);  // phase correct PWM mode
	//OCR0A = 0x10;                          // initial PWM pulse width

	//TCCR0B = (1 << CS01);   // clock source = CLK/8, start PWM

	//while (1)
	//{
	//	// change PWM pulse width every 2 seconds
	//	_delay_ms(2000);
	//	OCR0A = 0x10;
	//	_delay_ms(2000);
	//	OCR0A = 0x30;
	//	_delay_ms(2000);
	//	OCR0A = 0x50;
	//	_delay_ms(2000);
	//	OCR0A = 0xA0;
	//}

	///// 5

	//unsigned char PWM_val = 0;		// 8-bit PWM value
	//   unsigned char up_dn = 0;		// up down count flag
	//   
	//   DDRB   |= (1 << PB2);                   // PWM output on PB2
	//   TCCR0A = (1 << COM0A1) | (1 << WGM00);  // phase correct PWM mode
	//   TCCR0B = (1 << CS01);   // clock source = CLK/8, start PWM
	//
	//   while(1)
	//   {
	//       if ((PWM_val == 255) || (PWM_val == 0)) 
	//	{
	//           up_dn ^= 0x01;      // toggle count direction flag
	//       }
	//       
	//	_delay_ms(5);
	//       OCR0A  = PWM_val;       // write new PWM value
	//       
	//	if (up_dn) 
	//	{            // increment or decrement PWM value
	//           PWM_val++;
	//       }
	//       else 
	//	{
	//           PWM_val--;
	//       }
	//   }

	//////////// 6

	////PWM Initialisation

	//unsigned char i = 0;

	//TCCR1A = 0b10000001; // fast PWM mode 8-bit on OC1A
	//TCCR1B = 0b00001010; // prescaling by 8

	//					 //Initial value;
	//OCR1A = 0x00;

	//DDRB = 0xFF; // set port B for output
	////DDRB |= (1 << PB2);
	//			 // infinite loop
	//while (1)
	//{

	//	for (i = 0; i<255; i++)
	//	{
	//		OCR1A = i;
	//		_delay_ms(10);
	//	}

	//	for (i = 255; i>0; i--)
	//	{
	//		OCR1A = i;
	//		_delay_ms(10);
	//	}

	//}

	//////////////// 7

	////This is the first PWM register, TCNT0.  It is 8 bit.  Both PIN PA5 and PA6 are set to clear on compare,
	////then set at bottom; this makes them non-inverting.  The WGM bits are set to for "Fast PWM MODE"
	////and this clears at the top, "0x00FF."
	//TCCR0A = 0b10100011; // WORKS FOR OC0A, OC0B
	//TCCR0B = 0b00000001; // WORKS FOR OC0A, OC0B

	//					 //This is the second PWM register;TCNT1.  It is 8 bit.  Both PIN PB3 and PC0 are set to clear on compare,
	//					 //then set at bottom; this makes them non-inverting.  The WGM bits are set to for "Fast PWM MODE"
	//					 //and this clears at the top, "0x00FF."
	//TCCR1A = 0b10100001;  //WORKS FOR OC1A, OC1B
	//TCCR1B = 0b00001001;  //WORKS FOR OC1A, OC1B

	//					  //This sets the PWM pins as outputs.
	//DDRB |= (1 << PINB3);
	///*DDRA |= (1 << PINA5);
	//DDRA |= (1 << PINA6);
	//DDRC |= (1 << PINC0);*/

	//uint8_t brightness;


	////Setup several duty-cycle counters to show differential PWM channels.
	//uint8_t brightness2 = 0;
	//uint8_t brightness3 = 0;
	//uint8_t brightness4 = 0;

	////Let's only do this 3 times before turning PWM off.
	//for (int counterB = 0; counterB < 2; ++counterB) {

	//	//The main duty PWM cycle counter will also be our loop counter. (0-255)
	//	for (brightness = 255; brightness > 0; --brightness)
	//	{
	//		// set the brightness as duty cycle
	//		brightness2 = brightness2 + 1;
	//		brightness3 = brightness3 + 2;
	//		brightness4 = brightness4 + 10;

	//		OCR0A = brightness;   // PCO0
	//		OCR0B = brightness2;  // PA5
	//		OCR1A = brightness3;  // PB3
	//		OCR1B = brightness4;  // PA6

	//							  //Delay to make changes visible.
	//		_delay_ms(40);
	//	}

	//	//After 3 loops clear the PWM channels by setting COM0A1 and COM0B1 bits low.
	//	//If this is not done then there will be a nominal voltage on these pins due to
	//	//the internal pull-ups setting them as outputs.
	//	TCCR0A = 0b00000011; // WORKS FOR OC0A, OC0B
	//	TCCR1A = 0b00000011; // WORKS FOR OC0A, OC0B
	//}
}