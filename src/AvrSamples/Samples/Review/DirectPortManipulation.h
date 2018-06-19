#include <avr/io.h>			// This is our usual include
#define F_CPU 16000000UL    // This says to the compiler at what frequency our Atmega is running, in this case its 16Mhz
#include <util/delay.h>     // The delay functions/routines
#include "SerialUSART.h"

/*
Direct port manipulation, using the digital ports.

Well, to begin there is a dedicated register for each PORT that defines if each pin is a input or an output, 
that register is the DDRx, where x is the letter from the PORT that we want to configure, 
in the case of the Arduino there is DDRB, DDRC and DDRD. As every logic value, 
each bit in the DDRx registers can be either 1 or 0, 
being that putting a specific bit of DDRx at 1 configures the pin as output and putting it at 0 will configure the pin as an input.
*/

void DirectPortManipulationOverview(void)
void DirectPortManipulationOne(void);
uint8_t DirectPortManipulationDebounceButton(void);
void DirectPortManipulationTwo(void);

#define BitEquals(REG, bit, val)( ( (REG & (1UL << (bit) ) ) == ( (val) << (bit) ) ) )

void DirectPortManipulationOverview()
{
	// Configures pins 0,1,2,3 as digital inputs and pins 4,5,6,7 as digital outputs:
	DDRD = 0b11110000;

	// And all pins as outputs:
	DDRD = 0b11111111;

	/*
		There should be some care when using PORTD and Serial/USART because pins 0 and 1 from the PORTD 
		are the ones used by the USART and if you put both of them as inputs or outputs the USART 
		may be unable to read or write data in those pins.
	*/

	DDRD = 0b11111111;		// All pins in PORTD are outputs
	PORTD = 0b11111111;		// All pins in PORTD are high

	DDRD = 0b11111111;		// All pins in PORTD are outputs
	PORTD = 0b00000000;		// All pins in PORTD are low

	DDRD = 0b11111111;		// All pins in PORTD are outputs
	PORTD = 0b10101010;		// The small on,off,on,.. pattern

	DDRD = 0b00000000;		// All pins in PORTD are inputs
	uint8_t my_var = 0;		// Create a variable to store the data read from PORTD
	my_var = PIND;			// Read the PORTD and put the values in the variable

	DDRD = (1 << PD2);		// Pin 2 of portd is an output 000100
	PORTD = (1 << PD2);     // Pin 2 of portd as now the logic value 1

	// Or reading a button value:
	DDRD = 0b11111101;      // Pin 1 of PORTD is an input, all others are outputs
	my_var = 0;				// Create a variable to store the data read from PORTD
	my_var = (PIND & (1 << PD1));    // Read the PORTD pin 1 value and put it in the variable

	DDRD = 0b11111100;        // Portd pins 0 and 1 are inputs, all the others are outputs
	if (PIND & ((1 << PD0) | (1 << PD1))) 
	{
		// Code inside the if() statement will be executed when both buttons are high
	}

	DDRD = 0b00000000;		// All pins in PORTD are inputs
	PORTD = 0b00001111;		// Pull-ups enabled in the pins 0,1,2 and 3 and pull-ups disabled in pins 4,5,6 and 7
	my_var = 0;				// Create a variable to store the data read from PORTD
	my_var = PIND;			// Read the PORTD and put the values in the variable
}

void DirectPortManipulationOne(void)
{
	USART_Initialize();

	DDRD &= ~(1 << PD2);	// Configure PORTD pin 2 as an input
	PORTD |= (1 << PD2);    // Activate pull-ups in PORTD pin 2
	DDRB |= (1 << PB5);		// Configure PORTB pin 5 an output, this is the digital 13 in the Arduino that as the built-in led

	while (1)
	{
		_delay_ms(1000);

		USART_WriteChar(PIND);

		if (BitEquals(PIND, PD2, 1)) // If button is pressed
		{
			PORTB |= (1 << PC5);	// Turns ON LED
			_delay_ms(1000);		// Delay

			PORTB &= ~(1 << PC5);	// Turns OFF LED
			_delay_ms(1000);		// Delay
		}
	}
}

void DirectPortManipulationTwo(void)
{
	DDRD &= ~(1 << PD2);    // Configure PORTD pin 2 as an input
	PORTD |= (1 << PD2);	// Activate pull-ups in PORTD pin 2
	DDRB |= (1 << PB5);		// Configure PORTB pin 5 an output, this is the digital 13 in the Arduino that as the built-in led

	while (1) 
	{               
		if (DirectPortManipulationDebounceButton() == 1)
		{        
			// Verify the button state
			PORTB ^= (1 << PB5);    // This is the above mentioned XOR that toggles the led
		}
		_delay_ms(250);
	}
}

uint8_t DirectPortManipulationDebounceButton(void)
{
	// If the button was pressed delay a bit
	if (BitEquals(PIND, PD2, 1))
	{   
		_delay_ms(25);
	}        
	
	// Debounce the read value
	if (BitEquals(PIND, PD2, 1))  //Verify that the value is the same that what was read
	{
		return 1;
	}         
	// If it is still 0 its because we had a button press
	// If the value is different the press is invalid
	else
	{                  
		return 0;
	}
}