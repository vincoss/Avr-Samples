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

// Platform specific, MinUnit.h implementation. Write test output to USART or console
void MinUnitLogMessage(const char * str)
{
	printf("%s\n", str);
}