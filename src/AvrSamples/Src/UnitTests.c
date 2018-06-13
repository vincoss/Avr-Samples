/*
 * UnitTests.c
 *
 * Created: 20/04/2018 5:25:04 PM
 *  Author: Ferdinand Lukasak
 */ 


#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "MinUnit.h"
#include "UnitTests.h"
#include "Utility.h"


// These are required.
int MinUnitTestsRun = 0;
int MinUnitTestsSuccess = 0;
int MinUnitTestsFail = 0;

// Call form main.c.
int UnitTestsRun(void)
{
	MinUnitSetup();		// Basic setup, reset the tests counters
	
	MinUnitRunAll();	// Run the tests

	char buffer[100];
	snprintf(buffer, sizeof(buffer), "Tests failed: %d, passed: %d, of tests: %d", 
	MinUnitTestsFail, MinUnitTestsSuccess, MinUnitTestsRun);
	
	MinUnitLogMessage(buffer); // Print message
	
	return MinUnitTestsRun - MinUnitTestsSuccess;
}

// Initialize or reset values, called for each test
void MinUnitTestInitialize(void)
{
}

#pragma region Utility.h tests

void UtilityIsBitSetTest()
{
	uint8_t value = 0b00000010;

	MinUnitAssert(UtilityIsBitSet(value, 1) == 1, "Error:, UtilityIsBitSetTest data true");
	MinUnitAssert(UtilityIsBitSet(value, 0) == 0, "Error:, UtilityIsBitSetTest data false");
}

void UtilitySetBitAsUsedTest()
{
	uint8_t value = 0b00000000;
	UtilitySetBitAsUsed(&value, 1);

	MinUnitAssert(UtilityIsBitSet(value, 1) == 1, "Error:, UtilitySetBitAsUsedTest");
}

void UtilitySetBitAsUnUsedTest()
{
	uint8_t value = 0b00000010;
	UtilitySetBitAsUnUsed(&value, 1);

	MinUnitAssert(UtilityIsBitSet(value, 1) == 0, "Error:, UtilitySetBitAsUnUsedTest");
}

void UtilityFlipBitTest()
{
	uint8_t value = 0b00000000;
	UtilityFlipBit(&value, 1);

	MinUnitAssert(UtilityIsBitSet(value, 1) == 1, "Error:, UtilityFlipBitTest");
}

void IsNullOrEmptyTest()
{
	MinUnitAssert(IsNullOrEmpty(NULL) == 1, "Error:, IsNullOrEmptyTest null");
	MinUnitAssert(IsNullOrEmpty("") == 1, "Error:, IsNullOrEmptyTest empty");
	MinUnitAssert(IsNullOrEmpty("test") == 0, "Error:, IsNullOrEmptyTest not empty");
}

void IntToStringAndConvertToInt32Test()
{
	char buffer[12];
	long int  expected = INT32_MAX;
	char * format = "%ld";

	int returnValue = IntToString(expected, format, buffer, sizeof(buffer));
	long int actual = ConvertToInt32(buffer);

	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Error:, IntToStringAndConvertToInt32Test %ld == %ld, result: %d", expected, actual, expected == actual);
	MinUnitAssert(expected == actual, messageBuffer);
}

void IntToStringAndConvertToInt64Test()
{
	char buffer[20];
	long long int  expected = INT64_MAX;
	long long int actual = -1;
	char * format = "%lld";

	int returnValue = IntToString(expected, format, buffer, sizeof(buffer));
	actual = ConvertToInt(buffer, format);

	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Error:, IntToStringAndConvertToInt64Test %lld == %lld, result: %d", expected, actual, expected == actual);
	
	MinUnitAssert(expected == actual, messageBuffer);
}

void UnsignedIntToStringUnsignedAndConvertToInt32Test()
{
	char buffer[12];
	unsigned long int expected = UINT32_MAX;
	char * format = "%lu";

	UnsignedIntToString(expected, format, buffer, sizeof(buffer));
	unsigned long long int actual = ConvertToUnsignedInt(buffer, format);
	
	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Error:, UnsignedIntToStringUnsignedAndConvertToInt32Test %lu == %lu, result: %d", expected, actual, expected == actual);

	MinUnitAssert(expected == actual, messageBuffer);
}

void ConvertToUInt8Test()
{
	char buffer[8];
	int expected = UINT8_MAX;
	char * format = "%hhu";

	UnsignedIntToString(expected, format, buffer, sizeof(buffer));
	uint8_t actual = ConvertToUInt8(buffer);

	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Error:, ConvertToUInt8Test %hhu == %hhu, result: %d", expected, actual, expected == actual);

	MinUnitAssert(expected == actual, messageBuffer);
}

void ConvertToFloatTest()
{
	char buffer[20];
	float  expected = 3.141592;
	float actual = -1;
	char * format = "%f";

	int returnValue = FloatToString(expected, format, buffer, sizeof(buffer));
	actual = ConvertToFloat(buffer, format);

	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Error:, ConvertToFloatTest %.14f == %.14f, result: %d", expected, actual, expected == actual);
	
	MinUnitAssert((int)(expected * 1000) == (int)(actual * 1000), messageBuffer);
}

void ConvertToDoubleTest()
{
	char buffer[20];
	double  expected = 3.141592;
	double actual = -1;
	char * format = "%lf";

	int returnValue = FloatToString(expected, format, buffer, sizeof(buffer));
	actual = ConvertToDouble(buffer);

	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Error:, ConvertToDoubleTest %lf == %lf, result: %d", expected, actual, expected == actual);
	
	MinUnitAssert((int)(expected * 1000) == (int)(actual * 1000), messageBuffer);
}

#pragma endregion

// Register all tests in here
void MinUnitRunAll()
{
	// Utility.h
	MinUnitRun(UtilityIsBitSetTest);
	MinUnitRun(UtilitySetBitAsUsedTest);
	MinUnitRun(UtilitySetBitAsUnUsedTest);
	MinUnitRun(UtilityFlipBitTest);
	MinUnitRun(IsNullOrEmptyTest);
	MinUnitRun(IntToStringAndConvertToInt32Test);
	//MinUnitRun(IntToStringAndConvertToInt64Test);
	MinUnitRun(UnsignedIntToStringUnsignedAndConvertToInt32Test);
	MinUnitRun(ConvertToUInt8Test);

	MinUnitRun(ConvertToFloatTest);
	MinUnitRun(ConvertToDoubleTest);
}

