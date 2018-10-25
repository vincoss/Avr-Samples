/*
 * GPIOPorts_Samples.h
 *
 * Created: 18/07/2018 5:40:58 PM
 * Author: Ferdinand Lukasak
 */ 

/*

GPIO stands for general purpose input output.

ATMEGA32 has four ports, PORTA, PORTB, PORTC and PORTB

Every GPIO has three registers associated with it in order to control a particular pin. For AVR micro - controllers these registers are:

DDRn	Data Direction Register (Input or Output) Where 0
PORTn	Port Output data Register (High or Low)
PINn	Pin Input Register (PINn reads a port pin's current status)
n		Indicates the port name i.e. A, B, C & D

*/

#ifndef GPIO_PORTS_SAMPLES_H
#define GPIO_PORTS_SAMPLES_H

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif


#include <avr/io.h>		
#include <util/delay.h>
#include "Usart.h" 

#define BitEquals(REG, bit, val)( ( (REG & (1UL << (bit) ) ) == ( (val) << (bit) ) ) )

// TODO: replace those write to usart with UsartPrintf and test each, those pin values should be char or binary

void GPIOPortsSamples_One(void);

void GPIOPortsSamples_One(void)
{
	UsartInitialize(9600);

	DDRD &= ~(1 << PD2);	// Configure PORTD pin 2 as an input
	PORTD |= (1 << PD2);    // Activate pull-ups in PORTD pin 2
	DDRB |= (1 << PB5);		// Configure PORTB pin 5 an output, this is the digital 13 in the Arduino that as the built-in led

	while (1)
	{
		_delay_ms(1000);

		UsartPrintf("%d", PIND);

		if (BitEquals(PIND, PD2, 1)) // If button is pressed
		{
			PORTB |= (1 << PC5);	// Turns ON LED
			_delay_ms(1000);		// Delay

			PORTB &= ~(1 << PC5);	// Turns OFF LED
			_delay_ms(500);		// Delay
		}
	}
}

void GPIOPortsSamples_Two(void);
static uint8_t GPIOPortsSamples_DebounceButton(void);

void GPIOPortsSamples_Two(void)
{
	DDRD &= ~(1 << PD2);    // Configure PORTD pin 2 as an input
	PORTD |= (1 << PD2);	// Activate pull-ups in PORTD pin 2
	DDRB |= (1 << PB5);		// Configure PORTB pin 5 an output, this is the digital 13 in the Arduino that as the built-in led

	while (1)
	{
		if (GPIOPortsSamples_DebounceButton() == 1)
		{
			// Verify the button state
			PORTB ^= (1 << PB5);    // This is the above mentioned XOR that toggles the led
		}
		_delay_ms(250);
	}
}

uint8_t GPIOPortsSamples_DebounceButton(void)
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

void GPIOPortsSamples_DataDirectionRegisterSample(void);

void GPIOPortsSamples_DataDirectionRegisterSample(void)
{
	UsartInitialize(9600);

	while (1)
	{
		_delay_ms(2000);

		//USART_WriteChar(DDRB); // 000000000

		//DDRB = 0xFF;

		//USART_WriteChar(DDRB); // 001111111

		//DDRB = 0x00;

		//USART_WriteChar(DDRB); // 000000000

		DDRB |= 0x02;

		UsartWriteChar(DDRB); // 00000100
	}
}

void GPIOPortsSamples_PortOutputRegisterSample(void);

void GPIOPortsSamples_PortOutputRegisterSample(void)
{
	UsartInitialize(9600);

	while (1)
	{
		// Reset
		DDRB = 0x00;
		PORTB = 0x00;

		_delay_ms(2000);

		UsartWriteChar(DDRB); // 000000000

		// 1. To output data

		DDRB = 0xFF;  // Configure Port D as OUPUT

		UsartWriteChar(PORTB); // 000000000

		_delay_ms(50);

		PORTB = 0xFF; // Place Port D in High state

		UsartWriteChar(PORTB); // 00111111

		_delay_ms(50);

		// 2. To activate/deactivate internal pull-up registers

		DDRB = 0x00; // Port D configured as INPUT
		PORTB = 0xFF; // Pull-up registers activated on Port D

		UsartWriteChar(DDRB); // 00111111
	}
}

void GPIOPortsSamples_PinRegisterSample(void);

void GPIOPortsSamples_PinRegisterSample(void)
{
	UsartInitialize(9600);

	int led = 5;

	DDRB |= (1 << led);  // output

	while (1)
	{
		PORTB = 0;

		_delay_ms(2000);

		PORTB |= (1 << led);

		int value = PINB;

		UsartWriteChar(value);
	}
}

void GPIOPortsSamples_EnabledPullUpRegister_Led(void)
{
	DDRB |= (1 << PB5);     // output
							
	DDRB &= ~(1 << PB4);	// switch on pin 12 (PB4)	- Input
	PORTB |= (1 << PB4);	// enable pull-up resistor	- High

	while (1)
	{
		_delay_ms(2000);

		if (PINB & (1 << PB4))  // is switch open?
		{   
			PORTB &= ~(1 << PB5);   // switch is open, switch LED off
		}
		else 
		{
			PORTB |= (1 << PB5);    // switch is closed, switch LED on
		}
	}
}

void GPIOPortsSamples_DisablePullUpRegister_Led(void)
{
	DDRB |= (1 << PB5);     // output

	DDRB &= ~(1 << PB4);	// switch on pin 12 (PB4)	- Input
	PORTB &= ~(1 << PB4);	// enable pull-up resistor	- Low

	while (1)
	{
		_delay_ms(2000);

		if (PINB & (1 << PB4))  // is switch open?
		{
			PORTB &= ~(1 << PB5);   // switch is open, switch LED off
		}
		else
		{
			PORTB |= (1 << PB5);    // switch is closed, switch LED on
		}
	}
}

#pragma region Non runnable samples

void GPIOPortsSamples_Overview(void);

void GPIOPortsSamples_Overview(void)
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

	DDRD |= (1 << PD2);		// Pin 2 of PORTD is an output 000100
	PORTD |= (1 << PD2);     // Pin 2 of PORTD as now the logic value 1 HIGH

	// Or reading a button value:
	DDRD = 0b11111101;				// Pin 1 of PORTD is an input, all others are outputs
	my_var = 0;						// Create a variable to store the data read from PORTD
	my_var = (PIND & (1 << PD1));   // Read the PORTD pin 1 value and put it in the variable

	DDRD = 0b11111100;				// PORTD pins 0 and 1 are inputs, all the others are outputs
	if (PIND & ((1 << PD0) | (1 << PD1))) 
	{
		// Code inside the if() statement will be executed when both buttons are high
	}

	DDRD = 0b00000000;		// All pins in PORTD are inputs
	PORTD = 0b00001111;		// Pull-ups enabled in the pins 0,1,2 and 3 and pull-ups disabled in pins 4,5,6 and 7
	my_var = 0;				// Create a variable to store the data read from PORTD
	my_var = PIND;			// Read the PORTD and put the values in the variable
}

//  Data Direction Register configures data direction of a port or a port pin. I mean a port will used for input or output.
void GPIOPortsSamples_DDRxRegisterSample(void)
{
	// Configure PORT D as an output

	DDRB = 0xFF; // HEX notation
	// Or DDRB = 0b11111111; // BINARY NOTATION

	// Configure PORT D as an imput
	DDRB = 0x00; // HEX notation
	// Or DDRB = 0b00000000;

	// Configure second bit as an output
	DDRB |= 0x02;
}

// PORTn register has two purposes
// 1. To output data when port is configured as output (1 = High, 0 = Low)
// 2. To activate/deactivate internal pull-up registers when port is configures as input
void GPIOPortsSamples_PORTnRegisterSample(void)
{
	// 1. To output data
	// Set PORTB pins High
	DDRB = 0xFF;  // Configure Port D as OUPUT
	PORTB = 0xFF; // Place Port D in High state

	// 2. To activate/deactivate internal pull-up registers
	// Activate pull-up registers
	DDRB = 0x00; // Port D configured as INPUT
	PORTB = 0xFF; // Pull-up registers activated on Port D

	// Deactivate pull-up registers
	DDRB = 0x00;
	PORTB = 0x00; // Pull-up registers are deactivated on Port D
}

// The PINn register keeps the status of all the pins in that port.
void GPIOPortsSamples_PINnRegisterSample(void)
{
	// Get status of pin
	DDRB = 0x00; // Configure Port D as input
	uint8_t status = PIND; // Read status of all the 8 pins in status
}

#pragma endregion

#endif