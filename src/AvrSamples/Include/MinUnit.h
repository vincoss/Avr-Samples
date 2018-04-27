/*
 * MinUnit.h
 *
 * Created: 20/04/2018 5:23:07 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef MIN_UNIT_H_
#define MIN_UNIT_H_ 1

void MinUnitSetup(void);
void MinUnitTestInitialize(void);
void MinUnitRunAll(void);
void MinUnitLogMessage(const char * str);//printf("%s\n", message);

#define MinUnitAssert(message, test) do { if (!(test)) return message; } while (0)
#define MinUnitRun(test) do { MinUnitTestInitialize(); char * message = test(); MinUnitTestsRun++;  if (message) { MinUnitLogMessage(message); MinUnitTestsFail++; }else {MinUnitTestsSuccess++; }} while (0)

extern int MinUnitTestsRun;
extern int MinUnitTestsSuccess;
extern int MinUnitTestsFail;

void MinUnitSetup(void)
{
	MinUnitTestsRun = 0;	
	MinUnitTestsSuccess = 0;
	MinUnitTestsFail = 0;
}


/*
	EXAMPLE

	#include <stdio.h>
	#include "MinUnit.h"

	// These are required.
	int MinUnitTestsRun = 0;
	int MinUnitTestsSuccess = 0;
	int MinUnitTestsFail = 0;

	void MinUnitInitialize(void)
	{
		// Init here
	}

	static char * SampleTest()
	{
		MinUnitAssert("Error:, 1 + 1 = 2", (1 + 1) == 3);
		return 0;
	}

	static void RunAllTests()
	{
		MinUnitRun(SampleTest);
	}

	int UnitTestsMain()
	{
		RunAllTests();

		printf("Tests failed: %d, passed: %d, of tests: %d", MinUnitTestsFail, MinUnitTestsSuccess, MinUnitTestsRun);

		return MinUnitTestsRun - MinUnitTestsSuccess;
	}

*/

#endif