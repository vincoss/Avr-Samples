/*
* main.c
*
* Created: 20/04/2018 5:25:04 PM
*  Author: Ferdinand Lukasak
*/

#include <stdio.h>
#include "Utility.h"


int main(int argc, char *argv[])
{
	UnitTestsRun();

	printf("\nDone...");
	getchar();
	getchar();

	return 0;
}

// Platform specific, MinUnit.h implementation. Write test output to console or USART
void MinUnitLogMessage(const char * str)
{
	printf("%s\n", str);
}