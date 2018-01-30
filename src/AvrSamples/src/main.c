/*
 * AvrSamples.c
 *
 * Created: 29/01/2018 9:59:07 PM
 * Author : Ferdinand
 */ 


#ifndef F_CPU
# define F_CPU 16000000UL
#endif

#include "Adc.h"
#include "Usart.h"

//
//#include "BlinkingLedSamples.h"
//#include "SampleInclude.h"

// Warning implicit declaration of function [-Wimplicit-function-declaration]

int main(void)
{
	AdcInitialize();
	UsartInitialize();
	AdcSamples_CompleteConfig_Led();
	BlinkingLedSamples_SampleOne();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

