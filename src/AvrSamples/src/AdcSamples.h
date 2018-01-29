/*
 * AdcSamples.h
 *
 * Created: 29/01/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 


#ifndef ADCSAMPLES_H_
#define ADCSAMPLES_H_


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "Adc.h"
#include "Usart.h"

/*

An ADC has a resolution. A 10 Bit ADC has a range of 0-1023. (2^10=1024) The ADC also has a Reference voltage(ARef).
The ADC requires a frequency between 50KHz to 200KHz.
At higher frequency the conversion is fast while a lower frequency the conversion is more accurate.

Ref: https://hekilledmywire.wordpress.com/2011/03/16/using-the-adc-tutorial-part-5/
Now you are thinking, why would I want to reduce the maximum voltage of my ADC? Well this as a lot of uses,
but lets explain some things first. Using the default reference voltage of 5v and this voltage is converted to a 10 bits value,
the resolution of each bit is 5v/1023 = 4.88mV for each consecutive value, but imagine that you have an analogic accelerometer,
those are almost always 3.3v part, so if you used the 5v reference you would have 5 – 3.3 = 1.7v of unused values and also a reduced resolution,
now lets see what is the resolution if you used an 3.3v reference voltage 3.3/1023 = 3.22mV for each consecutive value,
so its a gain in resolution and you would be able to used the full range of the ADC.

*/

void AdcSamples_CompleteConfig_Led();

void AdcSamples_CompleteConfig_Led()
{
	// PB5/digital 13 is an output
	DDRB |= (1 << PB5);

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);

	// Set up the ADC use Port C / A0
	ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); // Prescaler at 128 so we have an 125Khz clock source
	ADMUX |= (1 << REFS0);	// Analog port Port 0
	ADMUX &= ~(1 << REFS1); // Avcc(+5v) as voltage reference

	// ADCSRB register
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));    // Analog Comparator, Free Running mode

	// ADCSRA register
	ADCSRA |= (1 << ADATE); // Bit 5 – ADATE: ADC Auto Trigger Enable
	ADCSRA |= (1 << ADEN);  // Bit 7 – ADEN: ADC Enable
	ADCSRA |= (1 << ADSC);	// Bit 6 – ADSC: ADC Start Conversion

	uint16_t value;

	while (1)
	{
		value = ADCW; // Read the ADC value, really that's just it

		if (value > 512)
		{
			PORTB |= (1 << PB5); //If ADC value is above 512 turn led on
		}
		else
		{
			PORTB &= ~(1 << PB5); //Else turn led off
		}
	}
}

void AdcSamples_WriteToUsart(void);

void AdcSamples_WriteToUsart(void)
{
	AdcInitialize();
	UsartInitialize();

	char buffer[5];

	while (1)
	{
		uint16_t value = AdcRead(5);	// Read the ADC value from analog port number 0-5

		itoa(value, buffer, 10);		// Convert the read value to an ascii string
		
		UsartWriteCharString(buffer);
		UsartWriteChar('\n');

		_delay_ms(500);
	}
}

void AdcSamples_Led(void);

void AdcSamples_Led(void)
{
	AdcInitialize();

	DDRB |= (1 << PB5);		// PB5/digital 13 is an output
	PORTB &= ~(1 << PB5);	// Set port to low to be safe

	uint16_t adcValue;	// 10 bits value

	while (1)
	{
		adcValue = AdcRead(1); // Read the ADC value from port number 0-5
		
		if (adcValue > 512)
		{
			PORTB |= (1 << PB5); //I f ADC value is above 512 turn led on
		}
		else
		{
			PORTB &= ~(1 << PB5); // Else turn led off
		}
	}
}


#endif /* ADCSAMPLES_H_ */