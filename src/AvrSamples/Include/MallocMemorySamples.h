#include <stdio.h>
#include <stdlib.h>
#include <avr/delay.h>
#include "Usart.h"

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
	char top;
	#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
	#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
	#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
	#endif  // __arm__
}

//int CheckFreeRam(void);
char * MallocMemorySamples_IntToString(long long int value, const char * format);

void MallocMemorySamples_Sample(void)
{
	uint8_t *_tester;

	while(1) 
	{
		uint8_t len = random_r(1000);
		UsartWriteCharString("-------------------------------------");
		UsartWriteChar('\n');
		
		UsartWriteCharString("Length: ");
		UsartWriteCharString(MallocMemorySamples_IntToString(len, "%d"));
		UsartWriteChar('\n');
		
		UsartWriteCharString("RAM: ");
		UsartWriteCharString(IntToString(freeMemory(), "%d"));
		UsartWriteChar('\n');
		////UsartWriteCharString("_tester = " + IntToString((uint16_t)_tester, "%d"));
		////UsartWriteCharString("alloating _tester memory");
		////_tester = (uint8_t *)malloc(len);
		////UsartWriteCharString("RAM: " + IntToString(CheckFreeRam(), "%d"));
		////UsartWriteCharString("_tester = " + IntToString((uint16_t)_tester, "%d"));
		////UsartWriteCharString("Filling _tester");
		////for (uint8_t i = 0; i < len; i++) {
			////_tester[i] = 255;
		////}
		////UsartWriteCharString("RAM: " + IntToString(CheckFreeRam(), "%d"));
		////UsartWriteCharString("freeing _tester memory");
		////free(_tester); _tester = NULL;
		////UsartWriteCharString("RAM: " + IntToString(CheckFreeRam(), "%d"));
		////UsartWriteCharString("_tester = " + IntToString((uint16_t)_tester, "%d"));
		_delay_ms(1000); // quick look
	}	
}

//int CheckFreeRam(void) 
//{
	//// found at learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
	//extern int __heap_start, *__brkval;
	//int v;
	//return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
//}

char * MallocMemorySamples_IntToString(long long int value, const char * format)
{
	if (strlen((format)) <= 0)
	{
		return "";
	}

	int length = snprintf(NULL, 0, format, value);
	char * str = malloc(length + 1);
	snprintf(str, length + 1, format, value);
	return str;
}