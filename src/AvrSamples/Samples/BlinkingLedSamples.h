/*
 * BlinkingLedSamples.c
 *
 * Created: 29/01/2018 10:06:57 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef BLINKING_LED_SAMPLES_H_
#define BLINKING_LED_SAMPLES_H_

#ifndef F_CPU
/*
	F_CPU tells the compiler that our crystal is an 16Mhz one so it can generate an accurate delay,
	must be declared above delay so delay knows what is the value of F_CPU.
*/
#define F_CPU 16000000UL
#endif

#include <avr/io.h>				// This is our usual include
#include <util/delay.h>			// The delay functions and routines
#include "Adc.h"

void BlinkingLedSamples_SampleOne(void);
void BlinkingLedSamples_SampleTwo(void);
void BlinkingLedSamples_SampleThree(void);
void BlinkingLedSamples_SampleFour(void);
void BlinkingLedSamples_SampleFive(void);
void BlinkingLedSamples_SampleSix(void);
void BlinkingLedSamples_SampleSeven(void);
void BlinkingLedSamples_SampleEight(void);
void BlinkingLedSamples_NaiveFadeLed(void);
void BlinkingLedSamples_PwmFadeLed(void);
void BlinkingLedSamples_LightSensor(void);
void BlinkingLedSamples_LightSensorAnalogWriteAndAdc(void);

/*

	The Arduino UNO has 3 ports

	The PORT register controls whether the pin is HIGH or LOW.
	PORTB B(digital pin 8 to 13) The two high bits (6 & 7) map to the crystal pins and are not usable
	PORTC C(analog input pins) Pins 6 & 7 are only accessible on the Arduino Mini
	PORTD D(digital pins 0 to 7)

	The DDR register, determines whether the pin is an INPUT or OUTPUT.
	The PORT register controls whether the pin is HIGH or LOW
	The PIN register reads the state of INPUT pins

	PB5
	P = Pin
	B = Port digital pins 8 to 13
	5 = decimal value (convert decimal 32 or binary 00100000)
 
 */

#define ledPinSampleOne PB5 // Set pin 13 to output. Press F12 key see definition and assigned value.

void BlinkingLedSamples_SampleOne(void)
{
	DDRB |= (1 << ledPinSampleOne); // pinMode(LED, OUTPUT); // sets the gitital pin as output

	while (1)
	{
		PORTB |= (1 << ledPinSampleOne); // digitalWrite(LED, ON); // turns the LED on
		_delay_ms(1000);

		PORTB &= ~(1 << ledPinSampleOne); // digitalWrite(LED, OFF); // turns the LED off
		_delay_ms(500);
	}
}

void BlinkingLedSamples_SampleTwo(void)
{
	int ledPin = 5; // pin 13

	DDRB |= (1 << ledPin); // sets the digital pin as output

	while (1)
	{
		PORTB |= (1 << ledPin); // digitalWrite HIGH
		_delay_ms(1000);

		PORTB &= ~(1 << ledPin); // digitalWrite LOW
		_delay_ms(2000);
	}
}

void BlinkingLedSamples_SampleThree(void)
{
	int ledPin = 32; // Just set with decimal value 32 or 00100000 in binary, pin 13

	DDRB = ledPin; // sets the digital pin as output. See don't need to use bit shift operator

	while (1)
	{
		PORTB |= ledPin; // digitalWrite HIGH
		_delay_ms(500);

		PORTB &= ~ledPin; // digitalWrite LOW
		_delay_ms(500);
	}
}

void BlinkingLedSamples_SampleFour(void)
{
	int ledPin = 0b111111; // Sets Arduino port B pins -,-,13,12,11,10,9,8 as outputs or decimal 63 with binary value

	DDRB = ledPin; // sets the digital pin as output

	while (1)
	{
		PORTB |= ledPin; // digitalWrite HIGH
		_delay_ms(500);

		PORTB &= ~ledPin; // digitalWrite LOW
		_delay_ms(500);
	}
}

void BlinkingLedSamples_SampleFive(void)
{
	DDRB |= (1 << PB5); // Define digital pin13/PORTB5 as an output so we can blink our led

	// This gives us an infinite loop, there should always be an infinite loop in your code,
	// because micro-controllers cant return from main to anywhere and that will give you bad results and unpredicted behaviour
	while (1)
	{
		PORTB |= (1 << PB5);	// Turn led on, this is the led included in the arduino(digital pin 13), Decimal: 32 Binary: 00100000
		_delay_ms(1000);		// Wait 1 second
		
		PORTB &= ~(1 << PB5);	//Turn led off
		_delay_ms(1000);		// Wait 1 second
	}
}

void BlinkingLedSamples_SampleSix(void)
{
	int pin = PB5;				// <avr/io.h>

	DDRB |= (1 << pin);			// Define digital pin13/PORTB5 as an output so we can blink our led

	while (1)
	{
		PORTB |= (1 << pin);    // Turn led on, this is the led included in the arduino(digital pin 13)
		_delay_ms(2000);		// Wait 2 seconds
		
		PORTB &= ~(1 << pin);   // Turn led off
		_delay_ms(2000);		// Wait 2 seconds
	}
}

void BlinkingLedSamples_SampleSeven(void)
{
	int ledPin = 63; // 00111111 Sets Arduino port B pins -,-,13,12,11,10,9,8 as outputs

	DDRB = ledPin; // Output

	while (1)
	{
		PORTB |= ledPin; // Sets Arduino port B pins -,-,13,12,11,10,9,8 HIGH
		_delay_ms(100);

		PORTB &= ~ledPin; // Sets Arduino port B pins -,-,13,12,11,10,9,8 LOW
		_delay_ms(100);
	}
}

void BlinkingLedSamples_SampleEight(void)
{
	// B0 pin to output then high and low

	DDRB = 0b00000001;	// Output

	while (1)
	{
		PORTB = 0b00000001;  // High
		_delay_ms(250);

		PORTB = 0b00000000;  // Low
		_delay_ms(1000);
	}
}

void BlinkingLedSamples_NaiveFadeLed(void)
{
	// Fade led with fast delay
	uint8_t ledPin = PB3; // 9

	DDRB |= (1 << ledPin); // output

	while (1)
	{
		PORTB |= (1 << ledPin);
		_delay_ms(2);

		PORTB &= ~(1 << ledPin);
		_delay_ms(8);
	}
}

void BlinkingLedSamples_PwmFadeLed(void)
{
	// Waveform Generation Mode - Fast PWM
	TCCR0A |= (1 << WGM00) | (1 << WGM01);

	// Compare Match Output Mode
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0);

	// Set prescaler to (no prescaling)
	TCCR0B |= (1 << CS00);

	// Make PD6 port output and make it low. OCR0A uses PD6 port. See Arduino port schema from more information.
	DDRD |= (1 << PD6);		// output
	PORTD &= ~(1 << PD6);	// low

	while (1)
	{
		// increasing brightness
		for (int brightness = 0; brightness < 255; ++brightness)
		{
			// set the brightness as duty cycle
			OCR0A = brightness;

			// delay so as to make the user "see" the change in brightness
			_delay_ms(5);
		}

		// decreasing brightness
		for (int brightness = 255; brightness > 0; --brightness)
		{
			// set the brightness as duty cycle
			OCR0A = brightness;

			// delay so as to make the user "see" the change in brightness
			_delay_ms(5);
		}
	}
}

// NOTE: Might use potentiometer instead of light sensor.
void BlinkingLedSamples_LightSensor(void)
{
	AdcInitialize();

	const int LED = PB5;
	const int AdcPort = 1; // To read light sensor value

	DDRB |= (1 << LED);	// output
	PORTB &= ~(1 << LED); // low

	int value = 0; // store analog value 0-1023

	while (1)
	{
		value = AdcRead(AdcPort);

		if(value <= 0)
		{
			value = 1000;
		}

		if(value >= 512)
		{
			PORTB |= (1 << LED);
			_delay_ms(1000);
		}
		else
		{
			PORTB &= ~(1 << LED);
			_delay_ms(1000);
		}
	}
}

void BlinkingLedSamples_LightSensorAnalogWriteAndAdc(void)
{
	// Configure PWM timer

	// Waveform Generation Mode - Fast PWM
	TCCR0A |= (1 << WGM00) | (1 << WGM01);

	// Compare Match Output Mode
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0);

	// Set prescaler to (no prescaling)
	TCCR0B |= (1 << CS00);

	AdcInitialize();

	int value = 0; // store analog value
	const int AdcPort = 1; // To read light sensor value

	// Make PD6 port output and make it low. OCR0A uses PD6 port. See arduion port schema from more information.
	DDRD |= (1 << PD6);
	PORTD &= ~(1 << PD6);

	while (1)
	{
		value = AdcRead(AdcPort); // Returns value 0-1023

		OCR0A = (value / 4); // set brightness max 255 so divide by 4
	}
}

#endif