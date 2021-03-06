/*
 * MinUnit.h
 *
 * Created: 20/04/2018 5:23:07 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef MIN_UNIT_H_
#define MIN_UNIT_H_ 1

void MinUnitSetup(void);							// For all tests
void MinUnitAssert(int test, const char * message);	// Assert test
void MinUnitRun(void callback(void));				// Run test
void MinUnitTestInitialize(void);					// Initialize or reset values, called for each test
void MinUnitRunAll(void);							// Run all tests
void MinUnitLogMessage(const char * str);			// Print error message

extern int MinUnitTestsRun;
extern int MinUnitTestsSuccess;
extern int MinUnitTestsFail;

int TestFailedAssertCount;	// Increment for each test failed assert.

void MinUnitSetup(void)
{
	MinUnitTestsRun = 0;	
	MinUnitTestsSuccess = 0;
	MinUnitTestsFail = 0;
	TestFailedAssertCount = 0;
}

void MinUnitAssert(int test, const char * message)
{
	if(test)
	{
		return; // All good
	}
	
	MinUnitLogMessage(message);
	TestFailedAssertCount++;
}

void MinUnitRun(void callback(void))
{
	MinUnitTestsRun++;  
	TestFailedAssertCount = 0; // Reset the assert for each test, one test might have multiple asserts.
	MinUnitTestInitialize();
	callback(); // Run actual test

	if (TestFailedAssertCount > 0)
	{
		MinUnitTestsFail++;
	}
	else
	{
		MinUnitTestsSuccess++;
	}
}

/*
	EXAMPLE

	#include <stdio.h>
	#include "MinUnit.h"

	// These are required.
	int MinUnitTestsRun = 0;
	int MinUnitTestsSuccess = 0;
	int MinUnitTestsFail = 0;

	void MinUnitLogMessage(const char * str)
	{
		printf("%s\n", str);
	}

	void MinUnitTestInitialize(void)
	{
		// Init for each test in here
	}

	void SampleTest()
	{
		MinUnitAssert((1 + 1) == 3, "Error:, 1 + 1 = 2");
	}

	static void MinUnitRunAll()
	{
		MinUnitRun(SampleTest);
	}

	int UnitTestsMain() // Call from main.c
	{
		MinUnitRunAll();

		printf("Tests failed: %d, passed: %d, of tests: %d", MinUnitTestsFail, MinUnitTestsSuccess, MinUnitTestsRun);

		return MinUnitTestsRun - MinUnitTestsSuccess;
	}

*/

#endif