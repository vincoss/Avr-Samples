/*
 *	MemorySamples.h
 *
 *	Created: 04/03/2018
 *  Author: Ferdinand Lukasak
 */ 


#ifndef MEMORY_SAMPLES_H_
#define MEMORY_SAMPLES_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/delay.h>
#include "Usart.h"

// TODO: Review all this.

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int MemorySamples_CheckFreenMemory();
char * MemorySamples_IntToString(long long int value, const char * format, char * buffer, int length);
void MemorySamples_Main(void);

void MemorySamples_Main(void)
{
	uint8_t *_tester;
	char buffer[100];
	
	UsartInitialize();

	while(1) 
	{
		uint8_t len = random_r(1000);
		UsartWriteCharString("-------------------------------------");
		UsartWriteChar('\n');
		
		UsartWriteCharString("Length: ");
		UsartWriteCharString(MemorySamples_IntToString(len, "%d", buffer, sizeof(buffer)));
		UsartWriteChar('\n');
		
		UsartWriteCharString("RAM: ");
		UsartWriteCharString(MemorySamples_IntToString(MemorySamples_CheckFreenMemory(), "%d", buffer, sizeof(buffer)));
		UsartWriteChar('\n');
		
		_delay_ms(1000);
	}	
}

char * MemorySamples_IntToString(long long int value, const char * format, char * buffer, int length)
{
	if (strlen((format)) <= 0)
	{
		return "";
	}
	memset(buffer, 0, length); // Clear
	snprintf(buffer, length, format, value);
	return buffer;
}

int MemorySamples_CheckFreenMemory()
{
	char top;
	#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
	#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
	#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
	#endif  // __arm__
}

#endif