
#include <avr/io.h>

void TempSample(void)
{
	// PB5/digital 13 is an output
	DDRB |= (1 << PB5);
	DDRB |= (1 << PB4);
	DDRB |= (1 << PB3);
	DDRB |= (1 << PB2);
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB0);
	//DDRD &= ~(1 << PB5);	// Input pinMode(BUTTON, INPUT); // and BUTTON is an input

	// Set it to low, just to be safe
	PORTB &= ~(1 << PB5);
	PORTB &= ~(1 << PB4);
	PORTB &= ~(1 << PB3);
	PORTB &= ~(1 << PB2);
	PORTB &= ~(1 << PB1);
	PORTB &= ~(1 << PB0);

	//PORTB |= (1 << PB5); // digitalWrite(LED, ON); // turns the LED on

	while (1)
	{

	}
}

//#include <avr/io.h>
//
//#define POWERTRAIN_L_MOTOR_START_STOP_PINB5		7	//5
//#define POWERTRAIN_R_MOTOR_START_STOP_PINB4		-1//6	//4
//// PLMSS
//
//void Sample(void)
//{
//	// TODO: create SPI samples file
//
//	// MISO
//	// MOSI
//	// SCK
//	// SS to start connection pull low for desired slave, after data send put SS line to high
//
//	// Enable interupt with SPIE) in the SPCR Register
//
//	// Set it to low, just to be safe
//	PORTB &= ~(1 << PB5);
//
//	while (1)
//	{
//
//		PORTB |= (1 << PB5); // digitalWrite(LED, ON); // turns the LED on
//	}
//}
//
//// MOSI sample
//
//#define SHIFT_REGISTER	DDRB
//#define SHIFT_PORT		PORTB
//#define DATA	(1<<PB3)	//MOSI (SI)
//#define LATCH	(1<<PB2)	//SS   (RCK)
//#define CLOCK	(1<<PB5)	//SCK  (SCK)
//
//void SN74HC595_IoInitialize(void)
//{
//	/*
//	TODO: check why is the SS pin input, low
//	Set MOSI and SCK output, all others input */
//	//DDRB = (1 << DD_MOSI) | (1 << DD_SCK);
//
//
//	SHIFT_REGISTER |= (DATA | LATCH | CLOCK);	//Set control pins as outputs
//	SHIFT_PORT &= ~(DATA | LATCH | CLOCK);		//Set control pins low
//}
//
//void SpiMasterInitialize(void)
//{
//	// Enable: SPI0, Master: MSTR0, clock rate fck/16: SPR0
//	SPCR0 = (1 << SPE) | (1 << MSTR0) | (1 << SPR0);
//}
//
//void SPI_MasterTransmit(unsigned char data)
//{
//	/* Start transmission */
//	SPDR0 = data;
//	/* Wait for transmission complete */
//	while (!(SPSR & (1 << SPIF)));
//}
//
//// MISO sample
//
//void SPI_SlaveInit(void)
//{
//	/* Set MISO output, all others input */
//	DDR_SPI = (1 << DD_MISO);
//	/* Enable SPI */
//	SPCR = (1 << SPE);
//}
//char SPI_SlaveReceive(void)
//{
//	/* Wait for reception complete */
//	while (!(SPSR & (1 << SPIF)))
//		;
//	/* Return Data Register */
//	return SPDR;
//}
