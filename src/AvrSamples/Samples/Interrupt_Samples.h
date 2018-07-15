/*
 * AdcSamples.h
 *
 * Created: 13/07/2018 10:08:17 PM
 *  Author: Ferdinand Lukasak
 */ 

#ifndef INTERRUPT_SAMPLES_H_
#define INTERRUPT_SAMPLES_H_

#include <avr/interrupt.h>
#include <util/atomic.h>

/*
	# Enable or disable global interrupts
	
	sei, which SEts the global Interrupt
	cli, which CLears the global Interrupt
	
	# When writing an Interrupt Service Routine (ISR):

	- Keep it short
	- Don't use delay ()
	- Don't do serial prints
	- Make variables shared with the main code volatile
	- Variables shared with main code may need to be protected by "critical sections" (see below)
	- Don't try to turn interrupts off or on
	
	# The main reasons you might use interrupts are:
	
	- To detect pin changes (eg. rotary encoders, button presses)
	- Watchdog timer (eg. if nothing happens after 8 seconds, interrupt me)
	- Timer interrupts - used for comparing/overflowing timers
	- SPI data transfers
	- I2C data transfers
	- USART data transfers
	- ADC conversions (analog to digital)
	- EEPROM ready for use
	- Flash memory ready
	
	# ATMega328P Interrupt list. Order is the higher up the list, the higher the priority.
	
	1  Reset
	2  External Interrupt Request 0  (pin D2)          (INT0_vect)
	3  External Interrupt Request 1  (pin D3)          (INT1_vect)
	4  Pin Change Interrupt Request 0 (pins D8 to D13) (PCINT0_vect)
	5  Pin Change Interrupt Request 1 (pins A0 to A5)  (PCINT1_vect)
	6  Pin Change Interrupt Request 2 (pins D0 to D7)  (PCINT2_vect)
	7  Watchdog Time-out Interrupt                     (WDT_vect)
	8  Timer/Counter2 Compare Match A                  (TIMER2_COMPA_vect)
	9  Timer/Counter2 Compare Match B                  (TIMER2_COMPB_vect)
	10  Timer/Counter2 Overflow                         (TIMER2_OVF_vect)
	11  Timer/Counter1 Capture Event                    (TIMER1_CAPT_vect)
	12  Timer/Counter1 Compare Match A                  (TIMER1_COMPA_vect)
	13  Timer/Counter1 Compare Match B                  (TIMER1_COMPB_vect)
	14  Timer/Counter1 Overflow                         (TIMER1_OVF_vect)
	15  Timer/Counter0 Compare Match A                  (TIMER0_COMPA_vect)
	16  Timer/Counter0 Compare Match B                  (TIMER0_COMPB_vect)
	17  Timer/Counter0 Overflow                         (TIMER0_OVF_vect)
	18  SPI Serial Transfer Complete                    (SPI_STC_vect)
	19  USART Rx Complete                               (USART_RX_vect)
	20  USART, Data Register Empty                      (USART_UDRE_vect)
	21  USART, Tx Complete                              (USART_TX_vect)
	22  ADC Conversion Complete                         (ADC_vect)
	23  EEPROM Ready                                    (EE_READY_vect)
	24  Analog Comparator                               (ANALOG_COMP_vect)
	25  2-wire Serial Interface  (I2C)                  (TWI_vect)
	26  Store Program Memory Ready                      (SPM_READY_vect)
	
	# Resources
	https://en.wikipedia.org/wiki/Interrupt
	https://www.microchip.com/webdoc/AVRLibcReferenceManual/porting_1iar_porting_isr.html
	https://gammon.com.au/interrupts
	https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
*/

/*

	TODO:
	attachInterrupt (0, switchPressed, CHANGE);    // that is, for pin D2
	attachInterrupt (digitalPinToInterrupt (2), switchPressed, CHANGE);

*/

void InterruptSample_EnableDisable()
{
	cli();	// Disable global interrupts
	
	// enable particular interrupt
	
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
}

/*
	# Disable interrupts if not sure if already disabled.
	At first save the current SREG (status register) which includes the interrupt flag. 
	After we have obtained the timer value (which is 4 bytes long) we put the status register back how it was.
*/
void IntrruptSamples_EnableDisableWithSreg()
{
	unsigned long long value;
	uint8_t oldSREG = SREG;
	
	/*
		Disable interrupts while reading multiple bytes or we might get an
		inconsistent value (e.g. in the middle of a write to value variable)
		See: Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_datasheet.pdf
		page: 33 for example code
	*/
	cli();
	value = 1111;
	SREG = oldSREG;
}

void InterruptSamples_ExternalInterruptClear()
{
	// See: Page 91.
	EIFR |= (1 << INTF0);  // clear flag for interrupt 0
	EIFR |= (1 << INTF1);  // clear flag for interrupt 1
}

// Example access with atomic
volatile uint8_t InterruptSamples_AtomicAccessValue = 0;

void InterruptSamples_AtomicAccess()
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		InterruptSamples_AtomicAccessValue = 123;
	}
}

/*
	Interrupt example with USART. Echo example just type something into the COM.
*/
void InterruptSamples_Usart()
{
	cli();	// Disable global interrupts
	
	// Just for the Usart configration
	#define LOCAL_F_CPU 16000000UL
	#define LOCAL_BAUDRATE 9600
	#define LOCAL_BAUD_PRESCALLER (((LOCAL_F_CPU / (LOCAL_BAUDRATE * 16UL))) - 1)
	
	// Usart configuration
	UBRR0H = (uint8_t)(LOCAL_BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(LOCAL_BAUD_PRESCALLER);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = ((1 << UCSZ00) | (1 << UCSZ01));

	UCSR0B |= (1 << RXCIE0); // Enable URSARTS receive interrupts

	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed

	while(1)
	{
		// See: ISR(USART_RX_vect)
	}
}

const int Interrupt_Samples_LED		= 5;
const int Interrupt_Samples_BUTTON	= 2;

/*
	This example shows how, even though the main loop is doing nothing, 
	you can turn the LED on pin 13 on or off, if the switch on pin D2 is pressed.

	To test this, just connect a wire (or switch) between D2 and Ground. 
	The internal pullup (enabled in setup) forces the pin HIGH normally. 
	When grounded, it becomes LOW. 
	The change in the pin is detected by a CHANGE interrupt, which causes the Interrupt Service Routine (ISR) to be called.
*/
void InterruptSamples_ButtonD2()
{
	// Setup
	DDRB |= (1 << Interrupt_Samples_LED);		// Output
	DDRD &= ~(1 << Interrupt_Samples_BUTTON);	// Input
	
	PORTB &= ~(1 << Interrupt_Samples_LED);		// Low
	PORTD |= (1 << Interrupt_Samples_BUTTON);	// High, internal pull-up resistor
	
	// Configure interrupt
	EICRA &= ~((1 << ISC01) | (1 << ISC00));	// clear existing flags
	EICRA |= (1 << ISC01);						// set wanted flags (Change level interrupt)
	EIMSK |= (1 << INT0);						// enable interrupt ISR(INT0_vect)
	
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	
	while(1)
	{
		
	}
}

void InterruptSamples_SwitchPressed()
{
	PORTB ^= (1 << PB5);    // XOR that toggles the led
}

ISR(USART_RX_vect)
{
	// Code to be executed when the USART receives a byte here
	char ReceivedByte;
	ReceivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	UDR0 = ReceivedByte; // Echo
}

ISR(INT0_vect)
{
	InterruptSamples_SwitchPressed();
}

ISR(BADISR_vect)
{
	// This is a vector which is aliased to __vector_default, the vector executed when an ISR fires with no accompanying ISR handler. This may be used along with the ISR() macro to create a catch-all for undefined but used ISRs for debugging purposes.
	
	//flash warning lamp
	//PORTB |= (1<<WARNING_LED_PIN);
	//set flag
	//Bad_Interrupt_Occured = true;
}

#endif