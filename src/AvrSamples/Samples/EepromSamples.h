/*
 * EepromSamples.h
 *
 * Created: 20/06/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef EEPROM_SAMPLES_H_
#define EEPROM_SAMPLES_H_

#include <stdio.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>
#include "Eeprom.h"
#include "Usart.h"


/*
	The ATmega32 contains 1024 bytes of data EEPROM memory. 
	It is organized as a separate data space. The EEPROM data bytes are addressed linearly between 0 and 1023.
	
	# EESAVE
	The EEPRPOM memory is preserved during Chip Erase if the EESAVE Fuse is programmed.
	The default value is not programed so then the EEPROM will be cleared if 'EESAVE' is not programmed.
		
	#Resources:
	https://www.avrfreaks.net/forum/tut-c-using-eeprom-memory-avr-gcc?page=all
*/


// Write & read value from EEPROM
void EepromSamples_WriteAndReadEepromValue(void);

void EepromSamples_WriteAndReadEepromValue(void)
{
	UsartInitialize();
	
	// Write
	uint8_t valueIn = 65;
	eeprom_update_byte((uint8_t *)46, valueIn); // Will update the value if changed

	// Read
	uint8_t valueOut = eeprom_read_byte((uint8_t *)46);

	while (1)
	{
		UsartWriteChar(valueOut);
		UsartWriteChar('\n');
		
		_delay_ms(1000);
	}
}

uint8_t EEMEM _sampleWithEEMEM_Keyword;

// Write & read value from EEPROM with EEMEM attribute
void EepromSamples_With_EEMEM_Keyword(void);

void EepromSamples_With_EEMEM_Keyword(void)
{
	UsartInitialize();
	
	eeprom_update_byte(&_sampleWithEEMEM_Keyword, 66); // Will update the value if changed
	
	// Read
	uint8_t valueOut = eeprom_read_byte(&_sampleWithEEMEM_Keyword);

	while (1)
	{
		UsartWriteChar(valueOut);
		UsartWriteChar('\n');
		
		_delay_ms(1000);
	}
}

// Sample with constant address value
void EepromSamples_WithAddressConstants(void);

void EepromSamples_WithAddressConstants(void)
{
	UsartInitialize();
	
	#define ADDRESS_1 46  // This could be anything from 0 to the highest EEPROM address
	#define ADDRESS_2 52  // This could be anything from 0 to the highest EEPROM address
	#define ADDRESS_3 68  // This could be anything from 0 to the highest EEPROM address

	uint8_t dataByte1 = 65;  // Data for address 1
	uint8_t dataByte2 = 66;  // Data for address 2
	uint8_t dataByte3 = 67;  // Data for address 3

	eeprom_update_byte((uint8_t*)ADDRESS_1, dataByte1);
	eeprom_update_byte((uint8_t*)ADDRESS_2, dataByte2);
	eeprom_update_byte((uint8_t*)ADDRESS_3, dataByte3);

	// Read
	uint8_t valueOut1 = eeprom_read_byte((uint8_t *)ADDRESS_1);
	uint8_t valueOut2 = eeprom_read_byte((uint8_t *)ADDRESS_2);
	uint8_t valueOut3 = eeprom_read_byte((uint8_t *)ADDRESS_3);

	while (1)
	{
		UsartWriteChar(valueOut1);
		UsartWriteChar('\n');
		
		UsartWriteChar(valueOut2);
		UsartWriteChar('\n');
		
		UsartWriteChar(valueOut3);
		UsartWriteChar('\n');
		
		_delay_ms(1000);
	}
}

typedef struct EepromSamplesLeds
{
	uint8_t PinA;
	uint8_t PinB;
} EL;

// Sample with struct
void EepromSamples_WithStruct(void);

void EepromSamples_WithStruct(void)
{
	UsartInitialize();
	
	EL write;
	EL read;
	
	write.PinA = 65;
	write.PinB = 66;

	eeprom_update_block((const void*)&write, (void*)0, sizeof(write));	// Address 0
	eeprom_read_block((void*)&read, (const void*)0, sizeof(read));		// Address 0
	
	while (1)
	{
		UsartWriteChar(read.PinA);
		UsartWriteChar('\n');
		
		UsartWriteChar(read.PinB);
		UsartWriteChar('\n');
		
		_delay_ms(1000);
	}
}

#pragma region Non runable examples

void EepromSamples_SampleOne(void)
{
	// Reading single byte of the data from EEPROM address 46

	uint8_t ByteOfData;
	ByteOfData = eeprom_read_byte((uint8_t *)46);
}

void EepromSamples_SampleTwo(void)
{
	// Reading two bytes of the data from EEPROM address 46
	uint16_t WordOfData;
	WordOfData = eeprom_read_word((uint16_t *)46);

	// Can use also these methods also, to read double words and floats
	// eeprom_read_float()
	// eeprom_read_dword()
}

void EepromSamples_SampleThree(void)
{
	// Writing single byte of th data to EEPROM	address 46

	uint8_t ByteOfData;
	ByteOfData = 0x12;
	eeprom_update_byte((uint8_t *)46, ByteOfData);
}

void EepromSamples_SampleFour(void)
{
	// Writing two bytes of the data to EEPROM address 46

	uint16_t WordOfData;
	WordOfData = 0x1232;	// 4658
	eeprom_update_word((uint16_t *)46, WordOfData);

	// Can use also these methods also, to write double words and floats
	// eeprom_update_float()
	// eeprom_update_dword()
}

void EepromSamples_SampleFive(void)
{
	// Read block of data from EEPROM address 12

	uint8_t StringOfData[10]; // 10 bytes
	eeprom_read_block((void *)StringOfData, (const void *)12, 10);
}

void EepromSamples_SampleSix(void)
{
	// Write block of data EEPROM address 12

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
uint8_t EepromSamples_ManualReadUtility(const uint8_t address)
{
	// Spin-lock until EEPROM finishes previous write
	while (EECR & (1<<EEPE));
	
	// Setup address register
	EEAR = address;
	
	// Start read
	EECR |= (1<<EERE);
	
	// Finishes right away
	return EEDR;
}

// Write a single byte to the given address. Duplicate checking, no error checking.
void EepromSamples_ManualWriteUtility(const uint8_t address, uint8_t value)
{
	// Save a needless write and spinlock on busy EEPROM (will not write if value is the same)
	if (value == EepromSamples_ManualReadUtility(address))
	{
		return;
	}
	
	// Setup address and data registers
	EEAR = address;
	EEDR = value;
	
	// Enable EEPROM write
	EECR |= (1<<EEMPE);
	
	// Start EEPROM write
	EECR |= (1<<EEPE);
}

void EepromSamples_Manual(void)
{
	UsartInitialize();
	
	uint8_t write = 66;
	uint8_t address = 0;
	
	EepromSamples_ManualWriteUtility(address, write);
	uint8_t read = EepromSamples_ManualReadUtility(address);
	
	while(1)
	{
		UsartWriteChar(read);
		UsartWriteChar('\n');
		
		_delay_ms(1000);
	}
}

void EepromSamples_WriteFirstAndLastByte()
{
	UsartInitialize();
	
	unsigned int eeprom_StartAddress = 0x00;	// 0
	unsigned int eeprom_EndAddress = 0x3ff;		// 1023
	
	// Write
	eeprom_update_byte((uint8_t *)eeprom_StartAddress, 65);
	eeprom_update_byte((uint8_t *)eeprom_EndAddress, 66);

	// Read
	uint8_t firstValue = eeprom_read_byte((uint8_t *)eeprom_StartAddress);
	uint8_t lastValue = eeprom_read_byte((uint8_t *)eeprom_EndAddress);
	
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
	
	unsigned int eeprom_address = 0x00;	// Address at 0
	unsigned char write_string[] = { "Hello World" }, read_string[15];
	 
	while(1)
	{
		UsartWriteCharString("\n\rWrite : ");             
		UsartWriteCharString(write_string);                
		eeprom_update_block((const void *)write_string, (void *)eeprom_address, sizeof(write_string)); // Write
		 
		UsartWriteCharString("\tRead : ");                 
		eeprom_read_block((void*)&read_string, (const void*)eeprom_address, sizeof(read_string));		// Read
		UsartWriteCharString(read_string);                 
		
		_delay_ms(1000);
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
	
	eeprom_update_word((uint16_t *)num_16_address, writeNum_16);	// write 2-bytes of data(writeNum_16) at 0x00.
	eeprom_update_dword((uint32_t *)num_32_address, writeNum_32);	// write 4-bytes of data(writeNum_32) at 0x02.
	
	readNum_16 = eeprom_read_word((uint16_t *)num_16_address);	// Read 2-bytes of data from 0x00 into readNum_16
	readNum_32 = eeprom_read_dword((uint32_t *)num_32_address);	// Read 4-bytes of data from 0x02 into readNum_32
	
	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Int16 = %d, Int32 = %ld", readNum_16, readNum_32);
	
	while (1)
	{
		UsartWriteCharString(messageBuffer);
		UsartWriteChar('\n');

		_delay_ms(1000);
	}
}

float EEMEM EEwriteFloat;

void EepromSamples_WriteFloat()
{
	UsartInitialize();
	
	float x = 123.1415;
	eeprom_update_block((const void*)&x, (void*)&EEwriteFloat, sizeof(float));

	float temp;
	eeprom_read_block((void*)&temp, (const void*)&EEwriteFloat, sizeof(float));
	
	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Float = %f", temp);
	
	while(1)
	{
		UsartWriteCharString(messageBuffer);
		UsartWriteChar('\n');
		
		_delay_ms(1000);
	}	
}

typedef struct StructSettings
{
	int8_t Int8;
	int16_t Int16;
	float Float;
	char Char[10];
} ST;

ST EEMEM EEStructSettings;

void EepromSamples_Struct()
{
	UsartInitialize();
	
	ST write;
	ST read;
	
	// Init
	write.Int8 = 10;
	write.Int16 = 516;
	write.Float = 123.45;
	strcpy(write.Char, "Hi there!");
	
	// Write
	eeprom_update_block((const void*)&write, (void*)&EEStructSettings, sizeof(ST));
	eeprom_read_block((void*)&read, (const void*)&EEStructSettings, sizeof(ST));
	
	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Int8 = %d, Int16 = %d, Float = %f, Char = %s" , read.Int8, read.Int16, read.Float, read.Char);
	
	while(1)
	{
		UsartWriteCharString(messageBuffer);
		UsartWriteChar('\n');

		_delay_ms(1000);
	}
}

/*
	Steps to write EEPROM with .eep file
	1. Build
	2. Write FLASH
		avrdude -p atmega328p -c arduino -P COM4 -b 57600 -D -U flash:w:main.hex
	3. Write EEPROM
		avrdude -p atmega328p -c arduino -P COM4 -b 57600 -D -U eeprom:w:main.eep
	4. Check for output
*/
uint8_t EEMEM _sampleWith_EEMEM_KeywordReadAndWriteValue = 66; // B character

void EepromSamples_With_EEMEM_KeywordReadAndWriteValue(void);

void EepromSamples_With_EEMEM_KeywordReadAndWriteValue(void)
{
	UsartInitialize();
	
	// Read
	uint8_t valueOut = eeprom_read_byte(&_sampleWith_EEMEM_KeywordReadAndWriteValue);

	while (1)
	{
		UsartWriteChar(valueOut);
		UsartWriteChar('\n');
		
		_delay_ms(1000);
	}
}


#endif