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
#include "Usart.h"

// TODO: Review and clean following samples.
/*
	Add example for string and other types use each method provide in eeprom.h header
	add a sample to fill whole eeprom
*/

/*
	The ATmega32 contains 1024 bytes of data EEPROM memory. 
	It is organized as a separate data space. The EEPROM data bytes are addressed linearly between 0 and 1023.
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


// Read a single byte from the given address
unsigned char EEPROM_read( unsigned int eeprom_addr ) {
	// Spin-lock until EEPROM finishes prev write
	while (EECR & (1<<EEPE)) ;
	// Setup address register
	EEAR = eeprom_addr;
	// Start read
	EECR |= (1<<EERE);
	// Finishes right away
	return EEDR;
}

// Write a single byte to the given address
// Duplicate checking, no error checking.
void EEPROM_write (unsigned int eeprom_addr, unsigned char eeprom_data) {
	// Save a needless write and spinlock on busy EEPROM
	if (eeprom_data == EEPROM_read(eeprom_addr))
	return;
	// Setup addr and data registers
	EEAR = eeprom_addr;
	EEDR = eeprom_data;
	// Enable EEPROM write
	EECR |= (1<<EEMPE);
	// Start EEPROM write
	EECR |= (1<<EEPE);
}

void EepromSamples_WriteFirstAndLastByte()
{
	UsartInitialize();
	
	unsigned int eeprom_StartAddress=0x00;
	unsigned int eeprom_EndAddress=0x3ff;
	
	// Write
	eeprom_update_byte(eeprom_StartAddress, 5);
	eeprom_update_byte(eeprom_StartAddress, 4);

	// Read
	uint8_t firstValue = eeprom_read_byte(eeprom_StartAddress);
	uint8_t lastValue = eeprom_read_byte(eeprom_EndAddress);
	
	while (1)
	{
		UsartWriteChar(firstValue);
		UsartWriteChar('\n');
		UsartWriteChar(lastValue);
		UsartWriteChar('\n');

		_delay_ms(1000);
	}
}

void EepromSamples_WriteString()
{
	UsartInitialize();
	
	 unsigned int eeprom_address=0x00;
	 unsigned char write_string[] = {"Hello World"}, read_string[15];
	 
	 while(1)
	 {
		 UsartWriteCharString("\n\rWrite : ");              // Print the message on UART
		 UsartWriteCharString(write_string);                // Print the String to be written
		 eeprom_write_byte(eeprom_address, write_string);	// Write the String at memory Location 0x00
		 
		 UsartWriteCharString("\tRead : ");                 // Print the message on UART
		 eeprom_read_block((void*)&read_string, (const void*)eeprom_address, sizeof(read_string));		// Read the String from memory Location 0x00
		 UsartWriteCharString(read_string);                 // Print the read String
	 }
}

void EepromSamples_WriteNumbers(void)
{
	UsartInitialize();
	
	uint16_t writeNum_16 = 12345;
	uint32_t writeNum_32 = 12345678;

	uint16_t readNum_16;
	uint32_t readNum_32;

	#define num_16_address 0x00
	#define num_32_address 0x02 // As num_16 takes two bytes, new address will start from +2 location
	
	eeprom_write_word((uint8_t *) num_16_address, writeNum_16);		// write 2-bytes of data(writeNum_16) at 0x00.
	eeprom_write_dword((uint8_t *) num_32_address, writeNum_32);	// write 4-bytes of data(writeNum_32) at 0x02.
	
	eeprom_read_word((uint16_t *)num_16_address);	// Read 2-bytes of data from 0x00 into readNum_16
	eeprom_read_dword((uint32_t *)num_32_address);	// Read 4-bytes of data from 0x02 into readNum_32
	
	//UART_Printf("num_16 = %5u    num_32=%8U",readNum_16,readNum_32);

	while (1);
}

#endif