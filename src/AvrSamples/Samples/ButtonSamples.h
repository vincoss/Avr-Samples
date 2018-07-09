/*
 * ButtonSamples.h
 *
 * Created: 29/01/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef BUTTON_SAMPLES_H_
#define BUTTON_SAMPLES_H_

#include <avr/io.h>				// This is our usual include
#include <util/delay.h>			// The delay functions and routines


#define BitEquals(Register, bit, val)( ( (Register & (1UL << (bit) ) ) == ( (val) << (bit) ) ) )
uint8_t DirectPortManipulationDebounceButton(void);
uint8_t IsBitSet(unsigned value, unsigned bitindex);
uint8_t ButtonDebounce(volatile uint8_t * registerAddress, uint8_t pinNo);

// Basic button example without button debounce
void ButtonSamples_HoldButtonToKeepLedOn(void)
{
	// Setup
	uint8_t ledPin = PB5;		// Port 13	- Decimal: 5,	Binary: 00100000
	uint8_t buttonPin = PD7;	// Port 7	- Decimal: 128, Binary: 10000000

	DDRB |= (1 << ledPin);		// Output pinMode(LED, OUTPUT); // tell Arduino LED is an output
	PORTB &= ~(1 << ledPin);	// Set it to low, just to be safe

	DDRD &= ~(1 << buttonPin);	// Input pinMode(BUTTON, INPUT); // and BUTTON is an input
	PORTD &= ~(1 << buttonPin);	// Set it to low, just to be safe

	while (1)
	{
		// Read value
		uint8_t currentValue = PIND;

		if (currentValue & (1 << buttonPin)) // HIGH/On
		{
			PORTB |= (1 << ledPin);
		}
		else
		{
			PORTB &= ~(1 << ledPin);
		}
	}
}

// Basic button example without button debounce
void ButtonSamples_KeepStateNormalOnOff(void)
{
	// Setup
	uint8_t ledPin = PB5;		// Port 13	- Decimal: 5,	Binary: 00100000
	uint8_t buttonPin = PD7;	// Port 7	- Decimal: 128, Binary: 10000000

	uint8_t state = 0;			// 0 = LED off and 1 = LED on

	DDRB |= (1 << ledPin);		// Output pinMode(LED, OUTPUT); // tell Arduino LED is an output
	PORTB &= ~(1 << ledPin);	// Set it to low, just to be safe

	DDRD &= ~(1 << buttonPin);	// Input pinMode(BUTTON, INPUT); // and BUTTON is an input
	PORTD &= ~(1 << buttonPin);	// Set it to low, just to be safe

	while (1)
	{
		// Read value
		uint8_t currentValue = PIND; 

		if (currentValue & (1 << buttonPin))
		{
			state = 1 - state;
			_delay_ms(25);
		}

		if (state == 1)
		{
			PORTB |= (1 << ledPin);
		}
		else
		{
			PORTB &= ~(1 << ledPin);
		}
	}
}

// Basic button example with button debounce
void ButtonSamples_DebounceOne(void)
{
	// Setup
	uint8_t ledPin = PB5;		// Port 13	- Decimal: 5,	Binary: 00100000
	uint8_t buttonPin = PD7;	// Port 7	- Decimal: 128, Binary: 10000000

	uint8_t currentValue = 0;	// sed to store the state of the input pin
	uint8_t oldValue = 0;		// stores the previous value of "val"
	char state = 0;				// 0 = LED off and 1 = LED on

	DDRB |= (1 << ledPin);		// Output pinMode(LED, OUTPUT); // tell Arduino LED is an output
	PORTB &= ~(1 << ledPin);	// Set it to low, just to be safe

	DDRD &= ~(1 << buttonPin);	// Input pinMode(BUTTON, INPUT); // and BUTTON is an input
	PORTD &= ~(1 << buttonPin);	// Set it to low, just to be safe

	while (1)
	{
		// Read value
		uint8_t currentValue = PIND;

		if (IsBitSet(currentValue, buttonPin) == 1 && IsBitSet(oldValue, buttonPin) == 0)
		{
			state = 1 - state;
			_delay_ms(25);
		}

		oldValue = currentValue; // val is now old, let's store it

		if (state == 1)
		{
			PORTB |= (1 << ledPin);
		}
		else
		{
			PORTB &= ~(1 << ledPin);
		}
	}
}

// Basic button example with button debounce
void ButtonSamples_DebounceTwo(void)
{
	// Setup
	uint8_t ledPin = PB5;		// Port 13	- Decimal: 5,	Binary: 00100000
	uint8_t buttonPin = PD7;	// Port 7	- Decimal: 128, Binary: 10000000

	uint8_t currentValue = 0;	// used to store the state of the input pin
	uint8_t oldValue = 0;		// stores the previous value of "val"
	char state = 0;				// 0 = LED off and 1 = LED on

	DDRB |= (1 << ledPin);		// Output pinMode(LED, OUTPUT); // tell Arduino LED is an output
	PORTB &= ~(1 << ledPin);	// Set it to low, just to be safe

	DDRD &= ~(1 << buttonPin);	// Input pinMode(BUTTON, INPUT); // and BUTTON is an input
	PORTD &= ~(1 << buttonPin);	// Set it to low, just to be safe

	// Loop
	while (1)
	{
		// Read value
		currentValue = PIND;

		//// check if there was a transition
		if (BitEquals(PIND, buttonPin, 1) == 1 && !(BitEquals(PIND, oldValue, 1)))
		{
			state = 1 - state;
			_delay_ms(25);
		}

		oldValue = currentValue; // val is now old, let's store it

		if (state == 1)
		{
			PORTB |= (1 << ledPin);
		}
		else
		{
			PORTB &= ~(1 << ledPin);
		}
	}
}

// Better way with button debounce
void ButtonSamples_DebounceThree(void)
{
	// Setup
	uint8_t ledPin = PB5;		// Port 13	- Decimal: 5,	Binary: 00100000
	uint8_t buttonPin = PD7;	// Port 7	- Decimal: 128, Binary: 10000000

	uint8_t currentValue = 0;	// used to store the state of the input pin
	uint8_t oldValue = 0;		// stores the previous value of "val"
	char state = 0;				// 0 = LED off and 1 = LED on

	DDRB |= (1 << ledPin);		// Output pinMode(LED, OUTPUT); // tell Arduino LED is an output
	PORTB &= ~(1 << ledPin);	// Set it to low, just to be safe

	DDRD &= ~(1 << buttonPin);	// Input pinMode(BUTTON, INPUT); // and BUTTON is an input
	PORTD &= ~(1 << buttonPin);	// Set it to low, just to be safe

	// Loop
	while (1)
	{
		if (ButtonDebounce(&PIND, buttonPin) == 1 && !(BitEquals(PIND, oldValue, 1)))
		{
			// Verify the button state
			PORTB ^= (1 << PB5);    // This is the above mentioned XOR that toggles the led
		}
	}
}

// NOTE: move into lib when done...
uint8_t ButtonDebounce(volatile uint8_t * registerAddress, uint8_t pinNo)
{
	// If the button was pressed delay a bit
	if (BitEquals(*registerAddress, pinNo, 1))
	{
		_delay_ms(25);
	}

	// Debounce the read value
	if (BitEquals(*registerAddress, pinNo, 1))  //Verify that the value is the same that what was read
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

uint8_t IsBitSet(unsigned value, unsigned bitindex)
{
	return ((value & (1 << bitindex)) != 0) ? 1 : 0;
}

#endif