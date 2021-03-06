/*
 * Utility.h
 *
 * Created: 29/01/2018 10:09:16 PM
 *  Author: Ferdinand Lukasak
 */ 


#ifndef UTILITY_H_
#define UTILITY_H_

#include <inttypes.h>

// TODO: some are fer other are by value, fix that
uint8_t UtilityIsBitSet(uint8_t address, uint8_t bitIndex);
void UtilitySetBitAsUsed(volatile uint8_t * address, uint8_t bitIndex);
void UtilitySetBitAsUnUsed(volatile uint8_t * address, uint8_t bitIndex);
void UtilityFlipBit(volatile uint8_t * address, uint8_t bitIndex);

int IsNullOrEmpty(const char * str);

int IntToString(long long int value, const char * format, char * buffer, int length);
int UnsignedIntToString(unsigned long long int value, const char * format, char * buffer, int length);

long int ConvertToInt32(const char * str);
long long int ConvertToInt(const char * str, const char * format);

uint8_t ConvertToUInt8(const char * str);
unsigned long long int ConvertToUnsignedInt(const char * str, const char * format);

int FloatToString(long double value, const char * format, char * buffer, int length);
float ConvertToFloat(const char * str, const char * format);
double ConvertToDouble(const char * str, const char * format);

#endif