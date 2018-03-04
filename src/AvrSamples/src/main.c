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
 */ 

#include "Adc.h"
#include "Usart.h"
#include <avr/delay.h>
#include "ConvertSamples.h"
#include "MemorySamples.h"
//b#include "Timer1_Sample1.h"
//#include "TimeSample.h"


int main(void)
{
	MemorySamples_Main();
	
	//while(1)
	//{
		////_delay_ms(1000);
	//}
}
