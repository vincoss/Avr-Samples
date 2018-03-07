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
#define DATA			(1<<PB3)	//MOSI (SI)
#define LATCH			(1<<PB2)	//SS   (RCK)
#define CLOCK			(1<<PB5)	//SCK  (SCK)

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

void SN74HC595_Sample(void)
{
	SN74HC595_IoInitialize();
	SN74HC595_SpiInitialize();

	unsigned int binary_counter = 0;

	while (1)
	{
		//Pull LATCH low (Important: this is necessary to start the SPI transfer!)
		SHIFT_PORT &= ~LATCH;
		
		// TODO: why two times? This should be changed to (1 << binary_counter)
		SN74HC595_SpiSend((unsigned char)(binary_counter >> 8));
		SN74HC595_SpiSend((unsigned char)binary_counter); // remove this one

		//Toggle latch to copy data to the storage register
		SHIFT_PORT |= LATCH;
		SHIFT_PORT &= ~LATCH; // TODO: do we need low again? ?? // Turn off the latch / disable

		//increment the counter for next time
		binary_counter++;
		
		// Just reset to see changes each loop cycle
		if(binary_counter >= 8)
		{
			binary_counter = 0;
		}

		//wait for a little bit before repeating everything
		_delay_ms(500);
	}
}


#endif /* SN74HC595_SAMPLES_H_ */