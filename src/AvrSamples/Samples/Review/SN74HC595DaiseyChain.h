#include <avr/io.h>
#include <util/delay.h>

//	How many of the shift registers are there daisey chained?
#define SN74HC595DaiseyChain_Number_Of_74hc595s 2
#define SHIFT_REGISTER	DDRB
#define SHIFT_PORT		PORTB
#define SERCLK			PB5 // Clock - SCK  (SCK)
#define RCLK			PB2	// Latch - SS   (RCK)
#define SER				PB3 // Data  - MOSI (SI)

void SN74HC595DaiseyChain_Sample(void)
{
	SHIFT_REGISTER |= (1 << SERCLK) | (1 << RCLK) | (1 << SER);	// set output
	SHIFT_PORT &= ~((1 << SERCLK) | (1 << RCLK) | (1 << SER));	// clear
	
	char counter = 0;

	while (1) 
	{
		counter++; 				// Counter used for displaying a number in binary via the shift register
		SN74HC595DaiseyChain_Shift(SERCLK, RCLK, SER, counter);		// PB5 = SERCLK  PB2 = RCLK  PB3 = SER
		_delay_ms(100);
		SN74HC595DaiseyChain_Shift(SERCLK, RCLK, SER, 0x00);		// Set all pins to off
		_delay_ms(100);
	}
}

void SN74HC595DaiseyChain_Shift(int SRCLK_Pin, int RCLK_Pin, int SER_Pin, unsigned long data)
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