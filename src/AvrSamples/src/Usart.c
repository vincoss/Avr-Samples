/*
 * Usart.c
 *
 * Created: 29/01/2018 10:10:40 PM
 *  Author: Ferdinand Lukasak
 */ 

#include <avr/io.h>
#include <stdarg.h>
#include "Usart.h"

//	PAGE: 228 external cloclk
void UsartInitialize(uint32_t baudRate)
{
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));
	UsartSetBaudRate(baudRate);
}

void UsartSetBaudRate(uint32_t baudRate)
{
	uint16_t registerValue;

	if((baudRate >= USART_MIN_BAUDRATE) && (baudRate <= USART_MAX_BAUDRATE))
	{
		registerValue = USART_BAUD_PRESCALLER(baudRate);
	}
	else
	{
		registerValue = USART_BAUD_PRESCALLER(USART_DEFAULT_BAUDRATE);
	}

	UBRR0L = (uint8_t)registerValue;		// Load lower 8- bits of the baud rate value into the low byte of the UBRR register
	UBRR0H = (uint8_t)registerValue >> 8;	// Load upper 8- bits of the baud rate value into the high byte of the UBRR register
}

void UsartWriteChar(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0))); // wait while register is free
	UDR0 = data; // load data in the register
}

unsigned char UsartReadChar(void)
{
	while (!(UCSR0A & (1 << RXC0))); // wait while data is being received
	return UDR0; // return 8-bit data
}

void UsartWriteCharString(const unsigned char* stringPtr)
{
	// Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null '\n' char
	while (*stringPtr != 0x00)
	{
		// Using the simple send function we send one char at a time
		UsartWriteChar(*stringPtr);

		// We increment the pointer so we can read the next char
		stringPtr++;
	}
}

void UsartInterruptInitialize(void)
{
	cli();	// Disable global interrupts
	UCSR0B |= (1 << RXCIE0); // Enable the USART Receive Complete interrupt ( USART_RX )
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
}

void UsartPrintf(const char * argList, ...) // TODO: comeplete this one
{
	va_list argp;
	va_start(argp, argList);
	
	const char *ptr;
	char *str;
	char  ch;
	
	switch('ch')
	{
		case 'C':
		case 'c':     /* Argument type is of char, hence read char data from the argp */
		{
			ch = va_arg(argp, int);
			UsartWriteChar(ch);
			break;
		}
		case  'S':
		case 's': /* Argument type is of string, hence get the pointer to sting passed */
		{
			str = va_arg(argp, char *);
			UsartWriteCharString(str);
			break;
		}
		default:
		{
			break;
		}
	}
}