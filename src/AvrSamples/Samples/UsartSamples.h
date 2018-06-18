/*
 * UsartSamples.h
 *
 * Created: 14/02/2018 7:46:39 AM
 *  Author: Ferdinand Lukasak
 */ 


#ifndef USART_SAMPLES_H_
#define USART_SAMPLES_H_

#include <avr/io.h>	
#include <avr/interrupt.h>
#include <util/delay.h>	
#include <stdlib.h>
#include "Usart.h"

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

void Usart_WriteCharSample(void)
{
	UsartInitialize();

	while (1)
	{
		for (uint8_t i = 0; i <= INT8_MAX; i++)
		{
			UsartWriteChar(i);

			_delay_ms(500);
		}

		UsartWriteChar('\r');
		UsartWriteChar('\n');
	}
}

void Usart_WriteStringSample(void)
{
	UsartInitialize();

	char string[] = "Hello AVR World!!!\n";

	while (1)
	{
		// Pass the string to the USART_WriteCharString function and sends it over the serial
		UsartWriteCharString(string);

		_delay_ms(1000);
	}
}

void Usart_WriteStringItoaSample(void)
{
	UsartInitialize();

	char buffer[5];
	int count = 0;

	while (1)
	{
		itoa(count, buffer, 10);		// Convert the read value to an ascii string

		count++;
		
		UsartWriteCharString(buffer);
		UsartWriteChar('\n');
		
		if(count == 1024)
		{
			
			count = 0;
		}

		_delay_ms(100);
	}
}

// Sample USART and interrupt. Use serial port to send a char or a string
void Usart_InterrupSample(void)
{
	UsartInitialize();

	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // And enable interrupts

	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed

	for (;;) // Loop forever
	{
	}
}

ISR(USART_RX_vect)
{
	// Code to be executed when the USART receives a byte here
	char ReceivedByte;
	ReceivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	UDR0 = ReceivedByte; // Echo
}

// TODO: Add sample for Usart and SPI (The USART can also be used in Master SPI mode.) page 254 in spec file

#endif