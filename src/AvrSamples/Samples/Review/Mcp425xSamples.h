#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "Usart.h"
#include "Mcp4xxx.h"

#define SCK_PB5		PB5 // SCK, CLK, SCLK (Slave clock)
#define MOSI_PB3	PB3	// IMOSI, SDI, SI, DI (Master Out Slave In)
#define MISO_PB4	PB4	// MISO, SDO, SO, DO (Master In Slave Out)
#define SS_PB2		PB2 // CS, SS  0 (Chip Select / Slave select)

void Mcp425xSamples_Initialize()
{
	// TODO: set SS_PB2 as input and keep it high, then disconnect the other wire

	DDRB = (1 << SS_PB2) | (1 << MOSI_PB3) | (1 << SCK_PB5);

	Mcp4xxxInitialize(&PORTB, SS_PB2);
}

// Increment
void Mcp425xSamples_Sample1(void)
{
	Mcp425xSamples_Initialize();

	Mcp4xxxSetBothWiper(0);

	uint16_t w0;
	uint16_t w1;

	char string[33];

	while (1)
	{
		_delay_ms(50);

		Mcp4xxxIncrementStep(0);
		Mcp4xxxIncrementStep(1);

		uint8_t result = Mcp4xxxRead(&w0, &w1); // read both wiper values

		if (result == 1)
		{
			// Error
		}

		if (w0 == 256 && w1 == 256)
		{
			Mcp4xxxSetBothWiperByPercent(0);
		}
	}
}

// SetStep
void Mcp425xSamples_Sample2()
{
	Mcp425xSamples_Initialize();

	uint8_t step = 1;
	unsigned int counter = 0;

	Mcp4xxxSetStep(0, 0);
	Mcp4xxxSetStep(1, 0);

	while (1)
	{
		_delay_ms(50);

		Mcp4xxxSetStep(step, counter);

		counter++;
		if (counter > 256)
		{
			counter = 0;
		}
	}
}

// Read
void Mcp425xSamples_Sample3()
{
	Mcp425xSamples_Initialize();

	uint16_t w0;
	uint16_t w1;

	Mcp4xxxSetStep(0, 128); // Set wiper 0 

	uint8_t value = Mcp4xxxRead(&w0, &w1); // read both wiper values

	while (1)
	{
		_delay_ms(1000);

		Mcp4xxxSetStep(1, w0); // set wiper 1
	}
}

// Tcon
void Mcp4251Samples_Sample4()
{
	UsartInitialize();
	Mcp425xSamples_Initialize();

	Mcp4xxxWriteTcon(0xFF); // save step value

	char string[33];

	while (1)
	{
		uint16_t value = Mcp4xxxReadTcon(); // read step value

		itoa(value, string, 10); // Convert the read value to an ascii string

		UsartWriteCharString(string);
		UsartWriteChar('\n');
	}
}