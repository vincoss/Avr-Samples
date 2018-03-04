/*
 * ConvertSample.c
 *
 * Created: 14/02/2018 7:46:39 AM
 *  Author: Ferdinand Lukasak
 */ 

#include "Usart.h"
#include "Utility.h"
#include <inttypes.h>

void ConvertSamples_To_uint8_t_FromString(void)
{
	uint8_t result = ConvertToUInt8("255");
	char * str = IntToString(result, "%hhu");
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

void ConvertSamples_To_Float_FromString(void)
{
	float result = ConvertToFloat("-3.1415");
	char * str = FloatToString(result, "%f");
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}

void ConvertSamples_To_Double_FromString(void)
{
	double result = ConvertToDouble("-3.14156");
	char * str = FloatToString(result, "%lf");
	
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}