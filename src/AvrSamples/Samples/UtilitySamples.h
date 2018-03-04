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
	char buffer[8];
	uint8_t result = ConvertToUInt8("255");
	char * str = IntToString(result, "%hhu", buffer, sizeof(buffer));
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

void UtilitySamples_To_Float_FromString(void)
{
	char buffer[8];
	float result = ConvertToFloat("-3.1415");
	char * str = FloatToString(result, "%f", buffer, sizeof(buffer));
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

void UtilitySamples_To_Double_FromString(void)
{
	char buffer[8];
	double result = ConvertToDouble("-3.1416");
	char * str = FloatToString(result, "%lf", buffer, sizeof(buffer));
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

#endif