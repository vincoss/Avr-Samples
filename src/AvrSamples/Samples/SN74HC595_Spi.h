#include <avr/io.h>

#define SHIFT_REGISTER	DDRB
#define SHIFT_PORT		PORTB
#define DATA	(1<<PB3)	// MOSI (SI, SER)
#define LATCH	(1<<PB2)    // SS   (RCK, RCLK)
#define CLOCK	(1<<PB5)    // SCK  (SCK, SRCLK)

void SN74HC595Spi_Sample(void)
{
	SHIFT_REGISTER |= (DATA | LATCH | CLOCK);	// Set control pins as outputs
	SHIFT_PORT &= ~(DATA | LATCH | CLOCK);		// Set control pins low

	// Setup SPI
	SPCR = (1 << SPE) | (1 << MSTR);  //Start SPI as Master

	//Pull LATCH low (Important: this is necessary to start the SPI transfer!)
	SHIFT_PORT &= ~LATCH;

	// Shift in some data
	SPDR = 0b10101010;  // This should light alternating LEDs

	//Wait for SPI process to finish
	while (!(SPSR & (1 << SPIF)));

	// Shift in some more data since I have two shift registers hooked up
	SPDR = 0b10101010;        //This should light alternating LEDs

	//Wait for SPI process to finish
	while (!(SPSR & (1 << SPIF)));

	// Toggle latch to copy data to the storage register
	SHIFT_PORT |= LATCH;
	SHIFT_PORT &= ~LATCH; // Turn off the latch / disable

	while (1)
	{
		//Loop forever
	}
}