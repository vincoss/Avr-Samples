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

/*

	USART:	A Universal Synchronous/Asynchronous Receiver/Transmitter.
	UART:	A Universal Asynchronous Receiver/Transmitter.

*/

void UsartSamples_Configuration()
{
	// These are required
	#define LOCAL_F_CPU 16000000UL
	#define LOCAL_BAUDRATE 9600
	#define LOCAL_BAUD_PRESCALLER (((LOCAL_F_CPU / (LOCAL_BAUDRATE * 16UL))) - 1)
	
	// Usart configuration
	UBRR0H = (uint8_t)(LOCAL_BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(LOCAL_BAUD_PRESCALLER);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));

	UCSR0B |= (1 << RXCIE0); // Enable receive interrupts
	
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	
	while(1)
	{
		// See: ISR(USART_RX_vect)
	}
}

void UsartSamples_WriteCharSample(void)
{
	UsartInitialize(9600);

	while (1)
	{
		for (uint8_t i = 0; i <= INT8_MAX; i++)
		{
			// NOTE: This does print right characters just wait and watch what is printed.
			UsartWriteChar(i);

			_delay_ms(50);
		}

		UsartWriteChar('\r');
		UsartWriteChar('\n');
	}
}

void UsartSamples_WriteStringSample(void)
{
	UsartInitialize(9600);

	char string[] = "Hello AVR World!!!\n";

	while (1)
	{
		// Pass the string to the USART_WriteCharString function and sends it over the serial
		UsartWriteCharString(string);

		_delay_ms(1000);
	}
}

void UsartSamples_WriteStringItoaSample(void)
{
	UsartInitialize(9600);

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

void UsartSamples_ReadWithLoopEcho(void)
{
	UsartInitialize(9600);
	char ReceivedByte;
	
	while(1)
	{
		while (( UCSR0A & (1 << RXC0 )) == 0) {}; // Do nothing until data have been received and is ready to be read from UDR
			
		ReceivedByte = UDR0 ; // Fetch the received byte value into the variable " ByteReceived "
		
		while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
		
		UDR0 = ReceivedByte; // Echo back the received byte back to the computer
	}
}

// Sample USART and interrupt. Use serial port to send a char or a string
void UsartSamples_InterrupSample(void)
{
	UsartInitialize(9600);

	cli();	// Disable global interrupts
	
	UCSR0B |= (1 << RXCIE0); // Enable interrupts

	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed

	for (;;) // Loop forever
	{
	}
}

ISR(USART_RX_vect)
{
	/*
		NOTE:
		When interrupt-driven data reception is used, the receive complete routine must read
		the received data from UDR in order to clear the RXC Flag, otherwise a new interrupt
		will occur once the interrupt routine terminates.
	*/
	
	// Code to be executed when the USART receives a byte here
	char ReceivedByte;
	ReceivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	UDR0 = ReceivedByte; // Echo
}

// TODO: Add sample for Usart and SPI (The USART can also be used in Master SPI mode.) page 254 in spec file.

#endif