/*
 * UnitTests.c
 *
 * Created: 20/04/2018 5:25:04 PM
 *  Author: Ferdinand Lukasak
 */ 


#include <stddef.h>
#include <stdint.h>
#include "MinUnit.h"
#include "UnitTests.h"


// These are required.
int MinUnitTestsRun = 0;
int MinUnitTestsSuccess = 0;
int MinUnitTestsFail = 0;

// Call form main.c.
int UnitTestsRun(void)
{
	MinUnitSetup();		// Basic setup, reset the counters
	
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

char * UtilityIsBitSetTest()
{
	uint8_t value = 0b00000010;

	MinUnitAssert("Error:, UtilityIsBitSetTest data true", UtilityIsBitSet(value, 1) == 1);
	MinUnitAssert("Error:, UtilityIsBitSetTest data false", UtilityIsBitSet(value, 0) == 0);

	return 0;
}

char * UtilitySetBitAsUsedTest()
{
	uint8_t value = 0b00000000;
	UtilitySetBitAsUsed(&value, 1);

	MinUnitAssert("Error:, UtilitySetBitAsUsedTest", UtilityIsBitSet(value, 1) == 1);

	return 0;
}

char * UtilitySetBitAsUnUsedTest()
{
	uint8_t value = 0b00000010;
	UtilitySetBitAsUnUsed(&value, 1);

	MinUnitAssert("Error:, UtilitySetBitAsUnUsedTest", UtilityIsBitSet(value, 1) == 0);

	return 0;
}

char * UtilityFlipBitTest()
{
	uint8_t value = 0b00000000;
	UtilityFlipBit(&value, 1);

	MinUnitAssert("Error:, UtilityFlipBitTest", UtilityIsBitSet(value, 1) == 1);

	return 0;
}

char * IsNullOrEmptyTest()
{
	MinUnitAssert("Error:, IsNullOrEmptyTest null", IsNullOrEmpty(NULL) == 1);
	MinUnitAssert("Error:, IsNullOrEmptyTest empty", IsNullOrEmpty("") == 1);
	MinUnitAssert("Error:, IsNullOrEmptyTest not empty", IsNullOrEmpty("test") == 0);

	return 0;
}

char * IntToStringAndConvertToInt32Test()
{
	char buffer[12];
	int expected = INT32_MAX;
	char * format = "%ld";

	IntToString(expected, format, buffer, sizeof(buffer));
	int actual = ConvertToInt32(buffer);

	char messageBuffer[100];
	snprintf(messageBuffer, sizeof(messageBuffer), "Error:, IntToStringAndConvertToInt32Test %ld == %ld, result: %d", expected, actual, expected == actual);
	MinUnitAssert(buffer, expected == actual);

	return 0;
}

char * UnsignedIntToStringUnsignedAndConvertToInt32Test()
{
	char buffer[12];
	unsigned int expected = UINT32_MAX;
	char * format = "%lu";

	UnsignedIntToString(expected, format, buffer, sizeof(buffer));
	unsigned int actual = ConvertToUnsignedInt32(buffer);

	MinUnitAssert("Error:, UnsignedIntToStringUnsignedAndConvertToInt32Test", expected == actual);

	return 0;
}

char * ConvertToUInt8Test()
{
	char buffer[8];
	int expected = UINT8_MAX;
	char * format = "%hhu";

	UnsignedIntToString(expected, format, buffer, sizeof(buffer));
	uint8_t actual = ConvertToUInt8(buffer);

	MinUnitAssert("Error:, UnsignedIntToStringUnsignedAndConvertToInt32Test", expected == actual);

	return 0;
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
	MinUnitRun(UnsignedIntToStringUnsignedAndConvertToInt32Test);
	MinUnitRun(ConvertToUInt8Test);
}
