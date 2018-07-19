/*
 * main.c
 *
 * Created: 29/01/2018 9:59:07 PM
 * Author : Ferdinand Lukasak
 
	 Copy main.hex into Arduino board use this command
	 1. Open command prompt
	 2. cd {SolutionDir}\debug
	 3. avrdude -p atmega328p -c arduino -P COM3 -b 115200 -D -U flash:w:main.hex
	 4. avrdude -p atmega328p -c arduino -P COM6 -b 57600 -D -U flash:w:main.hex
	 5. avrdude -p atmega328p -c arduino -P COM4 -b 57600 -D -U flash:w:main.hex
	 
 */ 
#include <avr/delay.h>
#include <inttypes.h>

// Lib
#include "Adc.h"
#include "Usart.h"

// Samples NOTE: Some samples are commented since the interrupt handlers are duplicated.
#include "AdcSamples.h"
//#include "Usart_Samples.h"
#include "BlinkingLedSamples.h"
#include "ButtonSamples.h"
#include "EepromSamples.h"
#include "Interrupt_Samples.h"
#include "MemorySamples.h"
//#include "Timer1_Sample1.h" // ISR is duplicated so this is commented out ISR(TIMER1_COMPA_vect)
//#include "TimeSample.h"
#include "SpiSamples.h"
#include "SN74HC595_Samples.h"
#include "UnitTests.h"
#include "DelayTCNT0_Sample.h"
#include "GPIOPorts_Samples.h"
#include "TIMER_Sample1.h"
#include "TIMER_Sample2.h"
#include "TIMER_Sample3.h"


int main(void)
{
	TIMER0_Sample3_Run();
	return 0;
}

/*
	// Main template
	int main(void)
	{
		while(1)
		{
			_delay_ms(1000);
		}
		
		return 0;
	}
*/

/*
// NOTE: Uncomment to run unit tests.
int main(void)
{
	UsartInitialize(9600);
	
	while(1)
	{
		UnitTestsRun();
		
		_delay_ms(3000);
	}
	
	return 0;
}
*/

// Platform specific, MinUnit.h implementation. Write test output to USART
void MinUnitLogMessage(const char * str)
{
	UsartWriteCharString(str);
	UsartWriteChar('\n');
}
