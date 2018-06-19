#include <avr/io.h>		
#include <util/delay.h>
#include "Usart.h" 

/* 

GPIO stands for general purpose input output. 

ATMEGA32 has four ports, PORTA, PORTB, PORTC and PORTD

Every GPIO has three registers associated with it in order to control a particular pin. For AVR micro - controllers these registers are:

DDRn	– Data Direction Register (Input or Output) Where 0 
PORTn	– Port Output data Register (High or Low)
PINn	– Pin Input Register (PINn reads a port pin's current status)
n		- Indicates the port name i.e. A, B, C & D

*/

void GPIOPortsSamples_DataDirectionRegisterSample(void);

void GPIOPortsSamples_DataDirectionRegisterSample(void)
{
	UsartInitialize();

	while (1)
	{
		_delay_ms(2000);

		//USART_WriteChar(DDRD); // 000000000

		//DDRD = 0xFF;

		//USART_WriteChar(DDRD); // 001111111

		//DDRD = 0x00;

		//USART_WriteChar(DDRD); // 000000000

		DDRD |= 0x02;

		UsartWriteChar(DDRD); // 00000100
	}
}

void GPIOPortsSamples_PortOutputRegisterSample(void);

void GPIOPortsSamples_PortOutputRegisterSample(void)
{
	UsartInitialize();

	while (1)
	{
		// Reset
		DDRD = 0x00;
		PORTD = 0x00;

		_delay_ms(2000);

		UsartWriteChar(DDRD); // 000000000

		// 1. To output data

		DDRD = 0xFF;  // Configure Port D as OUPUT

		UsartWriteChar(PORTD); // 000000000

		_delay_ms(50);

		PORTD = 0xFF; // Place Port D in High state

		UsartWriteChar(PORTD); // 00111111

		_delay_ms(50);

		// 2. To activate/deactivate internal pull-up registers

		DDRD = 0x00; // Port D configured as INPUT
		PORTD = 0xFF; // Pull-up registers activated on Port D

		UsartWriteChar(PORTD); // 00111111
	}
}

void GPIOPortsSamples_PinRegisterSample(void);

void GPIOPortsSamples_PinRegisterSample(void)
{
	UsartInitialize();

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

//  Data Direction Register configures data direction of a port or a port pin. I mean a port will used for input or output.
void GPIOPortsSamples_DDRxRegisterSample(void)
{
	// Configure PORT D as an output

	DDRD = 0xFF; // HEX notation
	// Or DDRD = 0b11111111; // BINARY NOTATION

	// Configure PORT D as an imput
	DDRD = 0x00; // HEX notation
	// Or DDRD = 0b00000000;

	// Configure second bit as an output
	DDRD |= 0x02;
}

// PORTn register has two purposes
// 1. To output data when port is configured as output (1 = High, 0 = Low)
// 2. To activate/deactivate internal pull-up registers when port is configures as input
void GPIOPortsSamples_PORTnRegisterSample(void)
{
	// 1. To output data
	// Set PORTD pins High
	DDRD = 0xFF;  // Configure Port D as OUPUT
	PORTD = 0xFF; // Place Port D in High state

	// 2. To activate/deactivate internal pull-up registers
	// Activate pull-up registers
	DDRD = 0x00; // Port D configured as INPUT
	PORTD = 0xFF; // Pull-up registers activated on Port D

	// Deactivate pull-up registers
	DDRD = 0x00;
	PORTD = 0x00; // Pull-up registers are deactivated on Port D
}

// The PINn register keeps the status of all the pins in that port.
void GPIOPortsSamples_PINnRegisterSample(void)
{
	// Get status of pin
	DDRD = 0x00; // Configure Port D as input
	uint8_t status = PIND; // Read status of all the 8 pins in status
}

#pragma endregion