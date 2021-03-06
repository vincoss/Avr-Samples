/*
 * SpiSamples.h
 *
 * Created: 7/03/2018 7:42:24 AM
 *  Author: Ferdinand Lukasak
 */ 


#ifndef SPI_SAMPLES_H_
#define SPI_SAMPLES_H_


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
	ATMEGA 328P SPI pins

	SCK		PB5/13
	MISO	PB4/12
	MOSI	PB3/11
	SS		PB2/10

	// SCK, CLK, SCLK, SRCLK (Clock, Slave Clock)
	// MOSI, SDI, SI, DI (Master Out Slave In)
	// MISO, SDO, SO, DO (Master In Slave Out)
	// CS, SS, RCK, RCLK, LATCH 0 (Chip Select / Slave select)
	// CS, SS, RCK, RCLK, LATCH 1 (Chip Select / Slave select)
*/

#define SET_SS_CS_LOW	(PORTB &= ~(1 << PB2))	// Activate slave	(Start bit line goes from 1 to 0)
#define SET_SS_CS_HIGH	(PORTB |= (1 << PB2))	// Deactivate slave (Stop bit(s) line goes from 0 to 1)

uint8_t SpiSamples_WriteRead(uint8_t data);
uint8_t SpiSamples_WriteReadBasic(uint8_t data);

void SpiSamples_MasterSlave()
{
	// SET SCK, MOSI and SS/Latch as output
	DDRB = (1 << PB5) | (1 << PB3) | (1 << PB2);

	// Set control pins low
	PORTB &= ~(PB5 | PB3 | PB2);

	// Enable SPI, Set Master, clock F_CPU/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

	uint8_t data = 0b00000111; // Set first three bits high

	SET_SS_CS_LOW; // If multiple slaves/servants are used then SS must be high, if only since slave is then keep it low.
	
	data = SpiSamples_WriteReadBasic(data); // Write data
	
	SET_SS_CS_HIGH; // Synchronize by pulling high

	while (1) 
	{
	}
}

uint8_t SpiSamples_WriteReadBasic(uint8_t dataOut)
{
	uint8_t dataIn;
	SPDR = dataOut; // Set data to send
	while (!(SPSR & (1 << SPIF))); // Wait for completion
	dataIn = SPDR; // Clear SPIF flag,
	return dataIn;
}

volatile int _counter;

// TODO: does not work, need to use two boards Master/Slave and then have an interrupt on the slave
#include "Utility.h"
// Try to set to low the watch to interrupt

void SpiSamples_MasterSlaveWithInterrupt()
{
	_counter = 0;
	
	// Set SCK, MOSI and SS/Latch as output
	DDRB = (1 << PB5) | (1 << PB3) | (1 << PB2);

	// Set control pins low
	PORTB &= ~(PB5 | PB3 | PB2);		

	SET_SS_CS_HIGH; // Deactivate slave (Must be high when not transferring)

	SPCR = ((1 << SPE)  |	// SPI Enable
			(0 << SPIE) |   // SPI Interupt Enable
			(0 << DORD) |   // Data Order (0:MSB first / 1:LSB first)
			(1 << MSTR) |   // Master/Slave select   
			(1 << SPR0)	|	// Clock F_CPU/16
			(0 << CPOL) |	// Clock Polarity (0:SCK low / 1:SCK hi when idle)
			(0 << CPHA));   // Clock Phase (0:leading / 1:trailing edge sampling)

	// TODO: In here set Speed and Interrupt, should be separate confiugration after SPI is enabled.

	sei(); // Enable global interrupt

	uint8_t i = 0;
	char buffer[8];

	while (1)
	{
		if (i == 256)
		{
			i = 0;
		}
		
		SpiSamples_WriteRead(i);
		
		IntToString(_counter, "%d", buffer, sizeof(buffer));
		UsartWriteCharString(buffer);
		UsartWriteChar('\n');

		i++;
		
		_delay_ms(50);
	}
}

ISR (SPI_STC_vect)
{
	_counter++;
}

uint8_t SpiSamples_WriteRead(uint8_t dataout)
{
	SET_SS_CS_LOW; // Activate slave

	uint8_t dataIn;
	
	SPDR = dataout; // Set data to send

	while (!(SPSR & (1 << SPIF))); // Wait for completion

	dataIn =  SPDR; // Clear SPIF flag, value is not required
	
	SET_SS_CS_HIGH; // Deactivate slave

	return dataIn;
}

#endif /* SPISAMPLES_H_ */