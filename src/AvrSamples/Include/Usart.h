/*
 * Usart.h
 *
 * Created: 29/01/2018 10:09:45 PM
 *  Author: Ferdinand Lukasak
 */ 


#ifndef USART_H_
#define USART_H_

// NOTE: For more information see specification document. RS-232

#ifndef F_CPU
# define F_CPU 16000000UL // Says to the compiler which is our clock frequency, permits the delay functions to be very accurate
#endif

#define USART_MIN_BAUDRATE		2400
#define USART_MAX_BAUDRATE		115200UL
#define USART_DEFAULT_BAUDRATE	9600   // The baudrate that we want to use

#define USART_BAUD_PRESCALLER(baudRate) (((( F_CPU / 16UL) + ( baudRate / 2)) / ( baudRate )) - 1)

void UsartInitialize(uint32_t baudRate);
void UsartSetBaudRate(uint32_t baudRate);
void UsartInterruptInitialize(void); // TODO: merge this with UsartInitialize

void UsartWriteChar(unsigned char data);
unsigned char UsartReadChar(void);
void UsartWriteCharString(const unsigned char* stringPtr);
void UsartPrintf(const char * str, ...);

#endif /* USART_H_ */