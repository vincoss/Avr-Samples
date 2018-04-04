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

void SN74HC595Spi_SampleOne(void)
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
	SPSR = (1 << SPI2X); // TODO:
	
	// Reset the 74HC595 register
	cnt = SN74HC595_ReadWriteExample_SPI_WriteRead(0);

	for (;;) 
	{
		cnt = 1;
		while (cnt) 
		{
			cnt = cnt << 1;
			PORTD = SN74HC595_ReadWriteExample_SPI_WriteRead(cnt);
			_delay_ms(100);
		}
		cnt = 0x80;
		while (cnt) 
		{
			cnt = cnt >> 1;
			PORTD = SN74HC595_ReadWriteExample_SPI_WriteRead(cnt);
			_delay_ms(100);
		}
	}
	return 0;
}

#include <stdlib.h>
#include <stdint.h>
typedef uint8_t bits_type;

#define CFG_SHIFT_DDR DDRB
#define CFG_SHIFT_PORT PORTB
#define CFG_SHIFT_SRCLK	PB1
#define CFG_SHIFT_RCLK	PB2
#define CFG_SHIFT_SER	PB3
#define CFG_SHIFT_REGISTER_PIN_COUNT	8


void SN74HC595_NoSpi_init(void)
{
	CFG_SHIFT_DDR |= _BV(CFG_SHIFT_RCLK)
	| _BV(CFG_SHIFT_SRCLK)
	| _BV(CFG_SHIFT_SER);
}

void SN74HC595_NoSpi_shift_bits_out(bits_type b, size_t len) 
{
	CFG_SHIFT_PORT &= ~_BV(CFG_SHIFT_RCLK);
	for (size_t i = 0; i < CFG_SHIFT_REGISTER_PIN_COUNT; i++) {
		CFG_SHIFT_PORT &= ~_BV(CFG_SHIFT_SRCLK);
		if (i < len && (b & ((bits_type)1 << i)))
		CFG_SHIFT_PORT |= _BV(CFG_SHIFT_SER);
		CFG_SHIFT_PORT |= _BV(CFG_SHIFT_SRCLK);
		CFG_SHIFT_PORT &= ~_BV(CFG_SHIFT_SER);
	}
	CFG_SHIFT_PORT |= _BV(CFG_SHIFT_RCLK);
}

int SN74HC595_NoSpi_Sample(void)
{
	SN74HC595_NoSpi_init();

	while (1) {
		SN74HC595_NoSpi_shift_bits_out(0x00, 8);
		_delay_ms(500);
		SN74HC595_NoSpi_shift_bits_out(0xff, 8);
		_delay_ms(500);
	}
	return 0;
}

//	How many of the shift registers are there daisey chained?
#define SN74HC595DaiseyChain_Number_Of_74hc595s 2
#define SHIFT_REGISTER	DDRB
#define SHIFT_PORT		PORTB
#define SERCLK			PB5 // Clock - SCK  (SCK)
#define RCLK			PB2	// Latch - SS   (RCK)
#define SER				PB3 // Data  - MOSI (SI)

void SN74HC595_DaiseyChain_Sample(void)
{
	SHIFT_REGISTER |= (1 << SERCLK) | (1 << RCLK) | (1 << SER);	// set output
	SHIFT_PORT &= ~((1 << SERCLK) | (1 << RCLK) | (1 << SER));	// clear
	
	char counter = 0;

	while (1)
	{
		counter++; 				// Counter used for displaying a number in binary via the shift register
		SN74HC595_DaiseyChain_Shift(SERCLK, RCLK, SER, counter);		// PB5 = SERCLK  PB2 = RCLK  PB3 = SER
		_delay_ms(100);
		SN74HC595_DaiseyChain_Shift(SERCLK, RCLK, SER, 0x00);		// Set all pins to off
		_delay_ms(100);
	}
}

void SN74HC595_DaiseyChain_Shift(int SRCLK_Pin, int RCLK_Pin, int SER_Pin, unsigned long data)
{
	SHIFT_PORT &= ~(1 << RCLK_Pin); 				// Set the register-clock pin low

	// Enter loop to shift out 8+ bits
	for (int i = 0; i < (8 * SN74HC595DaiseyChain_Number_Of_74hc595s); i++)
	{
		SHIFT_PORT &= ~(1 << SRCLK_Pin); 			// Set the serial-clock pin low

		SHIFT_PORT |= (((data&(0x01 << i)) >> i) << SER_Pin); 	// Go through each bit of data and output it

		SHIFT_PORT |= (1 << SRCLK_Pin); 			// Set the serial-clock pin high

		SHIFT_PORT &= ~(((data&(0x01 << i)) >> i) << SER_Pin);	// Set the datapin low again
	}

	SHIFT_PORT |= (1 << RCLK_Pin);				// Set the register-clock pin high to update the output of the shift-register
}

#endif /* SN74HC595_SAMPLES_H_ */