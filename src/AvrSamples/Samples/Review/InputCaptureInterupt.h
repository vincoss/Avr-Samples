#ifndef F_CPU
#define F_CPU 16000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>	
#include "Usart.h"	

/* Demodulation of PWM Signal from Analog Sensor 

*/

#define USER_LED PB5
#define ICP1_PIN PB0 // ICP - Input Capture Unit


void InputCaptureInterupt_SampleWithButton()
{
	UsartInitialize();

	DDRB &= ~(1 << ICP1_PIN);	// input
	PORTB |= (1 << ICP1_PIN);	// pullup enable

	// initialize timer

	TCNT1 = 0;
		TCCR1A = 0;
		TCCR1B = 0;
		TCCR1C = 0;
	
		/* Timer clock = I/O clock / 64 */
		TCCR1B = (1 << CS11) | (1 << CS10);
		TCCR1B |= (1 << ICNC1);
		TCCR1B |= (1 << ICNC1); // Bit 7 – ICNC1: Input Capture Noise Canceler (TODO: enable only if required)
		//TCCR1B |= (1 << ICES1); // Bit 6 – ICES1: Input Capture Edge Select (TODO: enable only if required)
	
		// TODO: See ICR1H and ICR1L registers

		/* Clear ICF1. Clear pending interrupts */
		TIFR1 = 1 << ICF1;
	
		/* Enable Timer 1 Capture Event Interrupt */
		TIMSK1 = 1 << ICIE1;
		sei();       // enable global interrupts.
		
}

ISR(TIMER1_CAPT_vect)
{
	TCNT1 = 0;
}

//void InputCaptureInterupt_Sample()
//{
//	DDRB |= (1 << USER_LED); // pinMode(LED, OUTPUT); // sets the gitital pin as putput
//	PORTB |= (1 << USER_LED);
//	//PORTB &= ~(1 << USER_LED);	// low
//
//	//DDRB &= ~(1 << ICP1_PIN);	// input
//	//PORTB |= (1 << ICP1_PIN);	// pullup enable
//	
//	//UsartInitialize();
//
//	////TCNT1 = 0;
//	////TCCR1A = 0;
//	////TCCR1B = 0;
//	////TCCR1C = 0;
//
//	/////* Timer clock = I/O clock / 64 */
//	////TCCR1B = (1 << CS11) | (1 << CS10);
//	////TCCR1B |= (1 << ICNC1);
//
//	/////* Clear ICF1. Clear pending interrupts */
//	////TIFR1 = 1 << ICF1;
//
//	/////* Enable Timer 1 Capture Event Interrupt */
//	////TIMSK1 = 1 << ICIE1;
//	//sei();       // enable global interrupts.
//	//TCCR1A = 0;      // normal mode
//	//TCCR1B = 68;     // (01000100) Rising edge trigger, Timer = CPU Clock/256 -or- 14745600/256 -or- 57.6KHz 
//	//TCCR1C = 0;      // normal mode
//	//TIMSK1 = 33;     // (00100001) Input capture and overflow interupts enabled
//
//	//TCNT1 = 0;       // start from 0
//
//
//	while (1)
//	{
//		//PORTB |= (1 << USER_LED); // digitalWrite(LED, ON); // turns the LED on
//
//		//_delay_ms(10);
//
//		//PORTB &= ~(1 << USER_LED);
//
//		//_delay_ms(500);
//	}
//}
//
//ISR(TIMER1_CAPT_vect)
//{
//	UsartWriteCharString("TIMER1_CAPT_vect");
//	UsartWriteChar('\n');
//	TCNT1 = 0;
//}



////// circular buffer for capture events
////#define   MAXR      8     // must be a power of 2. 8/16/32 ...
////#define   VALD      4     // to be considered as valid 2 successive measurement must have difference lower than VALD
////
////volatile  uint8_t   ring;
////volatile  int8_t    prot;
////volatile  uint16_t  icp[MAXR];
////
////volatile  uint16_t  pulseCount;
////volatile  uint8_t   intCount, edgeCount;
////#define USER_LED PB0
////
////void initCapture(void)
////{
////	// Input Capture setup
////	// ICNC1: Enable Input Capture Noise Canceler
////	// ICES1: =1 for trigger on rising edge
////	// CS12 CS11 CS10
////	//   0    0    1  : /1    No prescaler, CPU clock
////	//   0    1    0  : /8    prescaler
////	//   0    1    1  : /64   prescaler
////	//   1    0    0  : /256  prescaler
////	//   1    0    1  : /1024 prescaler
////
////	TCCR1A = 0;
////	TCCR1B = (1 << ICNC1) | (0 << ICES1) | (1 << CS11);
////	TCCR1C = 0;
////
////	// initialize to catch Falling Edge
////	{ TCCR1B &= ~(1 << ICES1); TIFR1 |= (1 << ICF1); }
////	ring = MAXR - 1; // so ring+1 -> MAXR -> 0
////	intCount = 0;
////	edgeCount = 0;
////
////	// Interrupt setup
////	// ICIE1: Input capture
////	// TOIE1: Timer1 overflow
////	TIFR1 = (1 << ICF1) | (1 << TOV1);    // clear pending interrupts
////	TIMSK1 = (1 << ICIE1) | (1 << TOIE1); // enable interupts
////
////										  // Set up the Input Capture pin, ICP1, Arduino Uno pin 8
////
////	//	DDRB |= (1 << USER_LED);
////	//	PORTB ^= (1 << USER_LED);
////	pinMode(8, INPUT);
////	digitalWrite(8, 0); // floating may have 50 Hz noise on it.
////						//digitalWrite(8, 1); // or enable the pullup
////}
////
////
////// to check there is a least 4 input change every 65536 count of timer
//////
////ISR(TIMER1_OVF_vect)
////{
////	edgeCount = intCount;
////	intCount = 0;
////}
////
////// Interrupt capture handler
//////
////ISR(TIMER1_CAPT_vect)
////{
////	ring = (ring + 1)&(MAXR - 1);
////
////	icp[ring] = ICR1;
////
////	if (prot == ring)       // if (prot==ring) there is a big problem. data will be overwritten. 
////		prot = -abs(ring);  // prot set to negative as error flag.
////
////							// setup to catch falling edge
////	if (ring & 1) { TCCR1B &= ~(1 << ICES1); TIFR1 |= (1 << ICF1); }
////	// setup to catch rising edge
////	else { TCCR1B |= (1 << ICES1); TIFR1 |= (1 << ICF1); }
////
////	pulseCount += (ring & 1) && pulseCount<0xFFFF;  //branch is bad when execution time is precious
////	intCount += intCount<0xFF;
////}
////
////
////
//////period is the time it takes a signal to repeat
////uint16_t Period(uint8_t edge)
////{
////	uint8_t   i;
////	uint16_t  val1, val2;
////
////	if ((intCount + edgeCount)>4)           // buff is full of readings
////	{
////		i = ring;                           // last writing in circular buffer
////		prot = (i + (MAXR - 5))&(MAXR - 1);       // protected area. TIMER1_CAPT_vect should not write here
////
////		if (edge ^ (i & 1))                    // if edge XOR rising, it's not the right one take previous
////			i = (i + (MAXR - 1))&(MAXR - 1);        // (i+3)&3 = i-1 range 0-3
////
////		val1 = icp[i] - icp[(i + (MAXR - 2))&(MAXR - 1)]; // (i+2)&3 = i-2 range 0-3
////		i = (i + (MAXR - 2))&(MAXR - 1);                  // (i+2)&3 = i-2 range 0-3
////		val2 = icp[i] - icp[(i + (MAXR - 2))&(MAXR - 1)]; // (i+2)&3 = i-2 range 0-3
////
////		if (prot >= 0 && abs(val1 - val2)<VALD) // No overwritting and 2 measurements quite same
////		{
////			return ((uint32_t)val1 + (uint32_t)val2) >> 1;  // Return mean        
////		}
////		else
////			return 0;                         // return 0 as error flag
////	}
////	else
////		return 0;                           // return 0 as error flag
////}
////
////
//////percentage of a period a signal is active 0-4095 = 0-100%
////int16_t Duty(uint8_t edge)
////{
////	uint8_t   i;
////	uint16_t  val1, val2;
////
////	if ((intCount + edgeCount)>4)           // buff is full of readings
////	{
////		i = ring;                           // last writing in circular buffer
////		prot = (i + (MAXR - 5))&(MAXR - 1);       // protected area. TIMER1_CAPT_vect should not write here
////
////		if (!edge ^ (i & 1))                   // if edge XOR rising, it's not the right one take previous
////			i = (i + (MAXR - 1))&(MAXR - 1);        // (i+3)&3 = i-1 range 0-3
////
////		val1 = icp[i] - icp[(i + (MAXR - 1))&(MAXR - 1)]; // (i+3)&3 = i-1 range 0-3
////		val1 = (((uint32_t)val1) << 12) / (icp[i] - icp[(i + (MAXR - 2))&(MAXR - 1)]);  // (i+2)&3 = i-2 range 0-3
////		i = (i + (MAXR - 2))&(MAXR - 1);          // (i+2)&3 = i-2 range 0-3
////		val2 = icp[i] - icp[(i + (MAXR - 1))&(MAXR - 1)]; // (i+3)&3 = i-1 range 0-3
////		val2 = (((uint32_t)val2) << 12) / (icp[i] - icp[(i + (MAXR - 2))&(MAXR - 1)]);  // (i+2)&3 = i-2 range 0-3
////
////																						// delay(4); // test for overwritting error
////
////		if (prot >= 0)                        // No overwritting
////		{
////			if (abs(val1 - val2)<VALD)          // 2 measurements quite same
////			{
////				return ((uint32_t)val1 + (uint32_t)val2) >> 1;  // Return mean        
////			}
////			else
////				return -2;                      // return -2 as error flag
////		}
////		else
////		{
////			return -1;                        // return -1 as error flag
////		}
////	}
////	else if (!(intCount + edgeCount))
////	{
////		if (!edge ^ digitalRead(8))
////			return 4095;
////		else
////			return 0;
////	}
////	else
////		return -3;                      // return -3 as error flag
////}
////
////uint16_t Count()
////{
////	uint16_t val = 0;
////
////	val = pulseCount;
////	pulseCount = 0;
////
////	return val;
////}
//
//uint8_t doubleOverflowError;
//
////typedef enum {
////	CAPTURE_1,
////	CAPTURE_2,
////	WAIT
////} timer_state_t;
////
////volatile timer_state_t flag = WAIT;
////
////// J:This is a 16-bit timer, so these values will always fit into an unsigned int
////volatile unsigned int Capt1, Capt2, CaptOvr;
////
////// J:Mind as well make this unsigned and give it 2x range since it can never be negative. 
////volatile unsigned long T1Ovs;
////
////void InitTimer1(void)
////{
////	//Set Initial Timer value
////	// J:All measurements against TCNT are relative, so no need to reset
////	// TCNT1=0;
////
////	// J: Note we need to set up all the timer control bits because we do not know what state they are in
////	// J: If, for example, the WGM bits are set to a PWM mode then the TCNT is going to be resetting out from under us rather than monotonically counting up to MAX
////
////	TCCR1A = 0x00;
////
////	//First capture on rising edge
////	TCCR1B = (1 << ICES1);
////	//Enable input capture and overflow interrupts
////	TIMSK1 |= (1 << ICIE1) | (1 << TOIE1);
////}
////
////// J: Note that it would be ok to start the timer when we assign TCCR1B in InitTimer since nothing will happen when the ISR is called until we set flag to CAPTURE1
////
////void StartTimer1(void)
////{
////	//Start timer without prescaler
////
////	// J: Note that we know that the other CS bits are 0 becuase of the Assignment in InitTimer
////	TCCR1B |= (1 << CS10);
////
////	//Enable global interrutps
////	// J: Interrupts are turned on by  Arduino platform startup code
////	//  sei();
////}
////
////ISR(TIMER1_CAPT_vect) {
////
////	switch (flag) {
////	case CAPTURE_1:
////		Capt1 = ICR1;
////
////		// J: Reset the overflow to 0 each time we start a measurement
////		T1Ovs = 0;
////		doubleOverflowError = 0;
////		flag = CAPTURE_2;
////		break;
////
////	case CAPTURE_2:
////		Capt2 = ICR1;
////
////		// J: Grab a snap shot of the overflow count since the timer will keep counting (and overflowing);
////		CaptOvr = T1Ovs;
////		flag = WAIT;
////
////		//J: Generally bad to print in ISRs
////		//Serial.println(flag);
////
////		break;
////	}
////}
////
////
////ISR(TIMER1_OVF_vect)
////{
////	T1Ovs++;
////
////	// J: Just to be correct, check for overflow of the overflow, otherwise if it overflows we would get an incorrect answer.
////	if (!T1Ovs) {
////		doubleOverflowError = 1;
////	}
////}
////
////void setup()
////{
////
////	//Serial.begin(9600);
////
////	InitTimer1();
////	StartTimer1();
////}
////
////void loop() {
////	// J: No need to bracket this set with cli() becuase the counter will not be counting until wait is updated
////
////	flag = CAPTURE_1;
////
////	while (flag != WAIT);
////
////
////	// J: Parenthesis and explicit cast for good luck! ( and to ensure correct size and order for operations) 
////
////
////	if (doubleOverflowError) {
////		//Serial.println("Double Overflow Error! Use a bigger prescaller!");
////	}
////	else {
////		//Serial.println(((unsigned long)(Capt2)+(CaptOvr * 0x10000UL)) - Capt1);
////	}
////}

//////////////////////////////////////////


//// tach.c
//// for NerdKits with ATmega168
//// nhusby@gmail.com
//
//#define F_CPU 20000000UL
//#define ICP PINB0
//
//// PIN DEFINITIONS:
////
//// ICP1 - (pin 14) Ignition pulse input capture
//// PC4  - (pin 27) 2Hz test pulse
//
//#include <stdio.h>
//#include <math.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
//#include <inttypes.h>
//#include "Usart.h"	
//#include <stdlib.h>		
//#include <util/delay.h>		
////#include "../libnerdkits/delay.h"
////#include "../libnerdkits/lcd.h"
//
//volatile uint16_t revTick;      // Ticks per revolution
//volatile uint16_t revCtr;           // Total elapsed revolutions
//
//void setupTimer()
//{           // setup timer1
//	TCCR1A = 0;      // normal mode
//	TCCR1B = 68;     // (01000100) Rising edge trigger, Timer = CPU Clock/256 -or- 14745600/256 -or- 57.6KHz 
//	TCCR1C = 0;      // normal mode
//	TIMSK1 = 33;     // (00100001) Input capture and overflow interupts enabled
//
//	TCNT1 = 0;       // start from 0
//}
//
//ISR(TIMER1_CAPT_vect)  // PULSE DETECTED!  (interrupt automatically triggered, not called by main program)
//{
//	//char buffer[5];
//	//itoa(ICR1, buffer, 10); // Convert the read value to an ascii string
//	//UsartWriteCharString("TIMER1_CAPT_vect");
//	//UsartWriteCharString(buffer);
//	//UsartWriteChar('\n');
//
//	revTick = ICR1;      // save duration of last revolution
//	TCNT1 = 0;       // restart timer for next revolution
//	revCtr++;                // add to revolution count
//}
//
//ISR(TIMER1_OVF_vect)    // counter overflow/timeout
//{
//	revTick = 0;
//}     // RPM = 0
//
//void IcmpSample()
//{
//	sei();       // enable global interrupts.
//	setupTimer();    // set timer perameters
//	UsartInitialize();
//	
//
//	PORTB |= (1 << ICP);     //pullup enabled
//	DDRB &= ~(1 << ICP);     //ICR1 as input
//	DDRB |= (1 << PB5);    // PB5 as output
//
//	uint16_t RPM;    // Revolutions per minute
//	char buffer[5];
//
//	while (1)
//	{  // do calculations and talk to lcd while we're not doing anything...
//
//		if (revTick > 0) // You're not Chuck Norris, DO NOT DIVIDE BY ZERO.
//		{
//			RPM = 3456000 / revTick; // 3456000 ticks/minute  
//		}     
//		else
//		{
//			RPM = 0;
//		}
//
//		itoa(RPM, buffer, 10); // Convert the read value to an ascii string
//		UsartWriteCharString("     RPM: ");
//		UsartWriteCharString(buffer);
//		UsartWriteChar('\n');
//
//		itoa(revTick, buffer, 10); // Convert the read value to an ascii string
//		UsartWriteCharString("Ticks/REV: ");
//		UsartWriteCharString(buffer);
//
//		itoa(revCtr, buffer, 10); // Convert the read value to an ascii string
//		UsartWriteCharString("revolutions: ");
//		UsartWriteCharString(buffer);
//
//		PORTB |= (1 << PB5);    // test signal  120RPM
//
//		_delay_ms(10);
//		PORTB &= ~(1 << PB5);
//
//		_delay_ms(499);    // wait 1/2 a second to update... I can't read that fast...
//	}
//
//}