/*
 * SN74HC595_Samples.h
 *
 * Created: 7/03/2018 8:02:00 AM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef SN74HC595_SAMPLES_H_
#define SN74HC595_SAMPLES_H_

#include <avr/io.h>
#include <util/delay.h>


#define SHIFT_REGISTER	DDRB
#define SHIFT_PORT		PORTB
#define DATA			(1<<PB3)	// MOSI (SI, SER)		PIN 14
#define LATCH			(1<<PB2)	// SS   (RCK, RCLK)		PIN 12
#define CLOCK			(1<<PB5)	// SCK  (SCK, SRCLK)	PIN 11

void SN74HC595_IoInitialize(void)
{
	SHIFT_REGISTER |= (DATA | LATCH | CLOCK);	//Set control pins as outputs
	SHIFT_PORT &= ~(DATA | LATCH | CLOCK);		//Set control pins low
}

void SN74HC595_SpiInitialize(void)
{
	// Enable SPI, Set Master, clock F_CPU/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);	// Start SPI as Master
}

uint8_t SN74HC595_SpiSend(uint8_t data)
{
	SPDR = data; // Shift in some data
	while (!(SPSR & (1 << SPIF)));	//Wait for SPI process to finish
	data = SPDR;
	return data;
}

void SN74HC595_Samples_One(void)
{
	SN74HC595_IoInitialize();
	SN74HC595_SpiInitialize();

	uint8_t binary_counter = 0;

	while (1)
	{
		// Just reset to see changes each loop cycle
		if(binary_counter == 255)
		{
			binary_counter = 0;
		}
		
		// Pull LATCH low (Important: this is necessary to start the SPI transfer!)
		SHIFT_PORT &= ~LATCH;
		
		SN74HC595_SpiSend(binary_counter);

		// Toggle latch to copy data to the storage register
		SHIFT_PORT |= LATCH;

		//increment the counter for next time
		binary_counter++;
		

		// wait for a little bit before repeating everything
		_delay_ms(50);
	}
}

void SN74HC595Spi_Samples_Complete(void)
{
	SHIFT_REGISTER |= (DATA | LATCH | CLOCK);	// Set control pins as outputs
	SHIFT_PORT &= ~(DATA | LATCH | CLOCK);		// Set control pins low

	// Enable SPI, Set Master
	SPCR = (1 << SPE) | (1 << MSTR);

	// Pull LATCH low (Important: this is necessary to start the SPI transfer!)
	SHIFT_PORT &= ~LATCH;

	// Shift in some data
	SPDR = 0b10101010;  // This should light alternating LEDs

	// Wait for SPI process to finish
	while (!(SPSR & (1 << SPIF)));

	// Shift in some more data since I have two shift registers hooked up
	SPDR = 0b01010101;  //This should light alternating LEDs

	//Wait for SPI process to finish
	while (!(SPSR & (1 << SPIF)));

	// Toggle latch to copy data to the storage register
	SHIFT_PORT |= LATCH;

	while (1)
	{
		//Loop forever
	}
}

#endif /* SN74HC595_SAMPLES_H_ */