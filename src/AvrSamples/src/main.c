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

#include "Adc.h"
#include "Usart.h"
#include <avr/delay.h>

// Samples
#include "AdcSamples.h"
#include "UsartSamples.h"
#include "BlinkingLedSamples.h"
#include "MemorySamples.h"
//#include "Timer1_Sample1.h" // ISR is duplicated so this is commented out ISR(TIMER1_COMPA_vect)
#include "TimeSample.h"
#include "UtilitySamples.h"
#include "SpiSamples.h"
#include "SN74HC595_Samples.h"
#include <inttypes.h>


char * IntToStringLocal(long long int value, const char * format, char * buffer, int length)
{
	if (IsNullOrEmpty(format) == 1)
	{
		return "";
	}
	memset(buffer, 0, length); // Clear
	snprintf(buffer, length, format, value);
	return buffer;
}

int main(void)
{
	UsartInitialize();
	
	char buffer[8];
	
	while(1)
	{
		//IntToStringLocal(255, "%ld", buffer, sizeof(buffer));
		UsartWriteCharString(IntToStringLocal(255, "%ld", buffer, sizeof(buffer)));
		
		_delay_ms(1000);
	}
	
	return 0;
}
