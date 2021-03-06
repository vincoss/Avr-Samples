/*
* Utility.h
*
* Created: 29/01/2018 10:09:16 PM
*  Author: Ferdinand Lukasak
*/


#include <stdio.h>
#include <inttypes.h>
#include "Utility.h"

uint8_t UtilityIsBitSet(uint8_t address, uint8_t bitIndex)
{
	return ((address & (1 << bitIndex)) != 0) ? 1 : 0;
}

void UtilitySetBitAsUsed(volatile uint8_t * address, uint8_t bitIndex)
{
	*address = (*address | 1 << bitIndex);
}

void UtilitySetBitAsUnUsed(volatile uint8_t * address, uint8_t bitIndex)
{
	*address = (*address & ~(1 << bitIndex));
}

void UtilityFlipBit(volatile uint8_t * address, uint8_t bitIndex)
{
	((*address) ^= (1 << (bitIndex)));
}

int IsNullOrEmpty(const char * str)
{
	if (str == NULL || !*str || *str == '\0')
	{
		return 1;
	}
	return 0;
}

/*
	NOTE: The snprintf and sscanf require other linker flags. '-Wl,-u,vfscanf -lprintf_flt -lscanf_flt'
*/

/*
	#Formats
	short				%hi
	unsigned short		%hu
	signed int			%i or %d
	unsigned int		%u
	signed long			%li
	unsigned long		%lu

	# These formats might not work on 8 bits
	long long			%lli or %lld
	unsigned long long	%llu
*/

int IntToString(long long int value, const char * format, char * buffer, int length)
{
	if (IsNullOrEmpty(format) == 1)
	{
		return -1; // Error
	}
	memset(buffer, 0, length); // Clear
	snprintf(buffer, length, format, value);
	return 0; // Success
}

int UnsignedIntToString(unsigned long long int value, const char * format, char * buffer, int length)
{
	if (IsNullOrEmpty(format) == 1)
	{
		return -1; // Error
	}
	memset(buffer, 0, length); // Clear
	snprintf(buffer, length, format, value);
	return buffer;
}

long long int ConvertToInt(const char * str, const char * format)
{
	if (IsNullOrEmpty(str) == 1)
	{
		return -1;
	}
	if (IsNullOrEmpty(format) == 1)
	{
		return -1;
	}
	long long int v;
	sscanf(str, format, &v);
	return v;
}

long int ConvertToInt32(const char * str)
{
	return ConvertToInt(str, "%ld");
}

unsigned long long int ConvertToUnsignedInt(const char * str, const char * format)
{
	if (IsNullOrEmpty(str) == 1)
	{
		return -1;
	}
	if (IsNullOrEmpty(format) == 1)
	{
		return -1;
	}
	unsigned long int v;
	sscanf(str, format, &v);
	return v;
}

uint8_t ConvertToUInt8(const char * str)
{
	if (IsNullOrEmpty(str) == 1)
	{
		return 0;
	}

	uint8_t v;
	sscanf(str, "%hhu", &v);
	return v;
}

/*
	#Formats
	float			%f, %g, %e, %a
	double			%lf, %lg, %le, %la
	long double		%Lf, %Lg, %Le, %La
*/
int FloatToString(long double value, const char * format, char * buffer, int length)
{
	if (IsNullOrEmpty(format) == 1)
	{
		return -1; // Error
	}
	memset(buffer, 0, length); // Clear
	snprintf(buffer, length, format, value);
	return 0;	// Success
}

float ConvertToFloat(const char * str, const char * format)
{
	if (IsNullOrEmpty(str) == 1)
	{
		return -1;
	}
	if (IsNullOrEmpty(format) == 1)
	{
		return -1; // Error
	}
	float v;
	sscanf(str, format, &v);
	return v;
}

double ConvertToDouble(const char * str, const char * format)
{
	if (IsNullOrEmpty(str) == 1)
	{
		return -1;
	}

	double v;
	sscanf(str, format, &v);
	return v;
}