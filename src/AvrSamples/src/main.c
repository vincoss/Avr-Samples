/*
 * main.c
 *
 * Created: 29/01/2018 9:59:07 PM
 * Author : Ferdinand Lukasak
 
	 Copy main.hex into Arduino board use this command
	 1. Open command prompt
	 2. cd {SolutionDir}\debug
	 3. avrdude -p atmega328p -c arduino -P COM3 -b 115200 -D -U flash:w:main.hex
 */ 

#include "Adc.h"
#include "Usart.h"
#include <avr/delay.h>
#include "ConvertSamples.h"
#include "MallocMemorySamples.h"

//
//uint8_t ConvertUInt8(const char * str);
////char * Convert(int value);
//char * ToString(int value, const char * format);


int main(void)
{
	UsartInitialize();
	
	MallocMemorySamples_Sample();
	
	//while(1)
	//{
		//_delay_ms(1000);
	//}
}

//
//uint8_t ConvertUInt8(const char * str)
//{
	////dtostrf()
	//uint8_t v;
	//sscanf(str, "%hhu", &v);
	//return v;
	//
	////uint8_t v = atoi(str);
	////return v;
//}
//
////char * Convert(int value)
////{
	////int length = snprintf( NULL, 0, "%d", value);
	////char* str = malloc(length + 1);
	////snprintf(str, length + 1, "%d", value);
	//////free(str); // TODO:
	////return str;
////}
//
//char * ToString(int value, const char * format)
//{
	//if(strlen((format)) <= 0)
	//{
		//return "";
	//}
	//
	//int length = snprintf(NULL, 0, format, value);
	//char* str = malloc(length + 1);
	//snprintf(str, length + 1, format, value);
	//return str;
//}