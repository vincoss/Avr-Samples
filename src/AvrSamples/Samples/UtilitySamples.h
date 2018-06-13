/*
 * UtilitySamples.h
 *
 * Created: 14/02/2018 7:46:39 AM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef UTILITY_SAMPLES_H_
#define UTILITY_SAMPLES_H_

#include "Usart.h"
#include "Utility.h"
#include <inttypes.h>

void UtilitySamples_To_uint8_t_FromString(void)
{
	char buffer[12];
	uint8_t result = ConvertToUInt8("255");
	char * str = IntToString(result, "%hhu", buffer, sizeof(buffer));
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

void UtilitySamples_To_int32_t_FromString(void)
{
	char buffer[12];
	long long int result = ConvertToInt32("2147483647");
	char * str = IntToString(result, "%ld", buffer, sizeof(buffer));
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

void UtilitySamples_To_uint32_t_FromString(void)
{
	char buffer[12];
	unsigned long long int result = ConvertToUnsignedInt32("4294967295");
	char * str = UnsignedIntToString(result, "%lu", buffer, sizeof(buffer));
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

void UtilitySamples_To_Float_FromString(void)
{
	char buffer[12];
	char * format = "%f";
	float result = ConvertToFloat("-3.1415", format);
	char * str = FloatToString(result, format, buffer, sizeof(buffer));
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

void UtilitySamples_To_Double_FromString(void)
{
	char buffer[12];
	char * format = "%lf";
	double result = ConvertToDouble("-3.1416", format);
	char * str = FloatToString(result, format, buffer, sizeof(buffer));
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

#endif