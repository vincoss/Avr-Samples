/*****************************************************************************
//  File Name    : SN74HC595_ReadWriteExample.h
//  Version      : 1.0
//  Description  : SPI I/O Using 74HC595 8-bit shift registers with output latch
//  Author       : FL
//  Target       : ATMEGA 328P
//  Compiler     : AVR-GCC WinAVR-20100110
//  IDE          :
//  Programmer   :
//               : AVR Visual Studio 2017
//  Last Updated : 28 May 2009
*****************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   PB2

unsigned char SN74HC595_ReadWriteExample_SPI_WriteRead(unsigned char dataout)
{
	unsigned char datain;
	// Start transmission (MOSI)
	SPDR = dataout;
	// Wait for transmission complete
	while (!(SPSR & (1 << SPIF)));
	// Get return Value;
	datain = SPDR;
	// Latch the Output using rising pulse to the RCK Pin
	SPI_PORT |= (1 << SPI_CS);
	_delay_us(1);             // Hold pulse for 1 micro second
							  // Disable Latch
	SPI_PORT &= ~(1 << SPI_CS);
	// Return Serial In Value (MISO)
	return datain;
}

int SN74HC595_ReadWriteExampleSample(void)
{
	unsigned char cnt;
	// Set the PORTD as Output:
	DDRD = 0xFF;
	PORTD = 0x00;

	// Initial the AVR ATMega168 SPI Peripheral
	// Set MOSI and SCK as output, others as input
	SPI_DDR = (1 << PB3) | (1 << PB5) | (1 << PB2);
	// Latch Disable (RCK Low)
	SPI_PORT &= ~(1 << SPI_CS);
	// Enable SPI, Master, set clock rate fck/2 (maximum)
	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR = (1 << SPI2X);
	// Reset the 74HC595 register
	cnt = SN74HC595_ReadWriteExample_SPI_WriteRead(0);

	for (;;) {
		cnt = 1;
		while (cnt) {
			cnt = cnt << 1;
			PORTD = SN74HC595_ReadWriteExample_SPI_WriteRead(cnt);
			_delay_ms(100);
		}
		cnt = 0x80;
		while (cnt) {
			cnt = cnt >> 1;
			PORTD = SN74HC595_ReadWriteExample_SPI_WriteRead(cnt);
			_delay_ms(100);
		}
	}
	return 0;
}