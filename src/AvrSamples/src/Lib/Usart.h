/*
 * Usart.h
 *
 * Created: 29/01/2018 10:09:45 PM
 *  Author: Ferdinand
 */ 


#ifndef USART_H_
#define USART_H_



#ifndef USART_H
#define USART_H

// NOTE: For more information see specification document. RS-232

#define F_CPU 16000000UL // Says to the compiler which is our clock frequency, permits the delay functions to be very accurate
#define USART_BAUDRATE 9600   // The baudrate that we want to use
#define USART_BAUD_PRESCALLER (((( F_CPU / 16UL) + ( USART_BAUDRATE / 2)) / ( USART_BAUDRATE )) - 1)

void UsartInitialize(void);
void UsartWriteChar(unsigned char data);
unsigned char UsartReadChar(void);
void UsartWriteCharString(unsigned char* StringPtr);

#endif


#endif /* USART_H_ */