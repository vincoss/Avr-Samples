/*
 * EepromSamples.h
 *
 * Created: 20/06/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef EEPROM_SAMPLES_H_
#define EEPROM_SAMPLES_H_

#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>
#include "Eeprom.h"

// TODO: Review and clean following samples.
/*
	Add example for string and other types use each method provide in eeprom.h header
*/

// Write & read value from EEPROM
void SampleWriteAndReadEepromValue(void);

void SampleWriteAndReadEepromValue(void)
{
	// Write
	uint8_t valueIn = 0;
	eeprom_update_byte((uint8_t *)46, valueIn); // Will update the value if changed

	// Read
	uint8_t valueOut = eeprom_read_byte((uint8_t *)46);

	// Set PB5 as output 13 and low
	DDRB |= (1 << valueOut);
	PORTB &= ~(1 << valueOut);

	while (1)
	{
		PORTB |= (1 << valueOut);
		_delay_ms(1000);

		PORTB &= ~(1 << valueOut);
		_delay_ms(500);
	}
}

uint8_t EEMEM _sampleWithEEMEM_Keyword;

// Write & read value from EEPROM with EEMEM attribute
void SampleWith_EEMEM_Keyword(void);

void SampleWith_EEMEM_Keyword(void)
{
	eeprom_update_byte(&_sampleWithEEMEM_Keyword, 5); // Will update the value if changed
	
	// Read
	uint8_t valueOut;
	valueOut = eeprom_read_byte(&_sampleWithEEMEM_Keyword);

	// Set PB5 as output 13 and low
	DDRB |= (1 << valueOut);
	PORTB &= ~(1 << valueOut);

	while (1)
	{
		PORTB |= (1 << valueOut);
		_delay_ms(1000);

		PORTB &= ~(1 << valueOut);
		_delay_ms(100);
	}
}

// TODO: This method does not work. Need to figure out how to upload .eep file first.
// Possible arduino uploader does not support it.
// avrdude -p atmega328p -c arduino -P COM3 -b 115200 -D -U flash:w:main.hex -U eeprom:w:main.eep:i
// Read
// avrdude -p atmega328p -c arduino -P COM3 -b 115200 -e -Ueeprom:r:main1.eep:i
// Upload
// avrdude -p atmega328p -c arduino -P COM3 -b 115200 -e -Ueeprom:w:main1.hex:i

// TODO: grab the .eep file with default value, then upload that into the controller and read the value.
uint8_t EEMEM _sampleWith_EEMEM_KeywordReadAndWriteValue = 5;

void SampleWith_EEMEM_KeywordReadAndWriteValue(void);

void SampleWith_EEMEM_KeywordReadAndWriteValue(void)
{
	// Read
	uint8_t valueOut = eeprom_read_byte(&_sampleWith_EEMEM_KeywordReadAndWriteValue);

	// Set PB5 as output 13 and low
	DDRB |= (1 << valueOut);
	PORTB &= ~(1 << valueOut);

	while (1)
	{
		PORTB |= (1 << valueOut);
		_delay_ms(1000);

		PORTB &= ~(1 << valueOut);
		_delay_ms(2000);
	}
}

// Sample with constant address value
void SampleWithAddressConstants(void);

void SampleWithAddressConstants(void)
{
	#define ADDRESS_1 46  // This could be anything from 0 to the highest EEPROM address
	#define ADDRESS_2 52  // This could be anything from 0 to the highest EEPROM address
	#define ADDRESS_3 68  // This could be anything from 0 to the highest EEPROM address

	uint8_t dataByte1 = 5;  // Data for address 1
	uint8_t dataByte2 = 4;  // Data for address 2
	uint8_t dataByte3 = 3;  // Data for address 3

	eeprom_update_byte((uint8_t*)ADDRESS_1, dataByte1);
	eeprom_update_byte((uint8_t*)ADDRESS_2, dataByte2);
	eeprom_update_byte((uint8_t*)ADDRESS_3, dataByte3);

	// Read
	uint8_t valueOut = eeprom_read_byte((uint8_t *)ADDRESS_1);

	// Set PB5 as output 13 and low
	DDRB |= (1 << valueOut);
	PORTB &= ~(1 << valueOut);

	while (1)
	{
		PORTB |= (1 << valueOut);
		_delay_ms(1000);

		PORTB &= ~(1 << valueOut);
		_delay_ms(500);
	}
}

struct EepromSamplesLeds
{
	int PinA;
	int PinB;
};

// Sample with struct
void EepromSamples_WithStruct(void);

void EepromSamples_WithStruct(void)
{
	struct EepromSamplesLeds in;
	struct EepromSamplesLeds out;

	in.PinA = 5;
	in.PinB = 4;

	eeprom_write_block((const void*)&in, (void*)0, sizeof(in));
	eeprom_read_block((void*)&out, (const void*)0, sizeof(out));
	
	uint8_t port = out.PinA;

	// Set PB4 as output 12 and low
	DDRB |= (1 << in.PinA);
	PORTB &= ~(1 << in.PinA);

	while (1)
	{
		PORTB |= (1 << port);
		_delay_ms(1000);

		PORTB &= ~(1 << port);
		_delay_ms(500);
	}
}

#pragma region Non runable examples

void EepromSamples_SampleOne(void)
{
	// Reading single byte of the data from EEPROM

	uint8_t ByteOfData;
	ByteOfData = eeprom_read_byte((uint8_t *)46);
}

void EepromSamples_SampleTwo(void)
{
	// Reading two bytes of the data from EEPROM
	uint16_t WordOfData;
	WordOfData = eeprom_read_word((uint16_t *)46);

	// Can use also these methods also, to read double words and floats
	// eeprom_read_float()
	// eeprom_read_dword()
}

void EepromSamples_SampleThree(void)
{
	// Writing single byte of th data to EEPROM

	uint8_t ByteOfData;
	ByteOfData = 0x12;
	eeprom_update_byte((uint8_t *)46, ByteOfData);
}

void EepromSamples_SampleFour(void)
{
	// Writing two bytes of the data to EEPROM

	uint16_t WordOfData;
	WordOfData = 0x1232;	// 4658
	eeprom_update_word((uint16_t *)46, WordOfData);

	// Can use also these methods also, to write double words and floats
	// eeprom_update_float()
	// eeprom_update_dword()
}

void EepromSamples_SampleFive(void)
{
	// Read block of data

	uint8_t StringOfData[10]; // 10 bytes
	eeprom_read_block((void *)StringOfData, (const void *)12, 10);
}

void EepromSamples_SampleSix(void)
{
	// Write block of data

	char StringOfData[10] = "TEST";
	eeprom_update_block((const void *)StringOfData, (void *)12, 10);
}

// Use of EEMEM attribute for automatic address assign by compiler.

uint8_t EEMEM NonVolatileChar;
uint16_t EEMEM NonVolatileInt;
uint8_t EEMEM NonVolatileString[10];

void EepromSamples_SampleSeven()
{
	uint8_t SRAMchar;
	uint16_t SRAMint;
	uint8_t SRAMstring[10];
	
	SRAMchar = eeprom_read_byte(&NonVolatileChar);
	SRAMint = eeprom_read_word(&NonVolatileInt);
	eeprom_read_block((void *)SRAMstring, (const void *)NonVolatileString, 10);
}

// Setting default EEPROM values. For this sample must write main.eep file into EEPROM

uint8_t EEMEM SomeVariable = 12;

#pragma endregion

#endif