#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t bits_type;

#define CFG_SHIFT_DDR DDRB
#define CFG_SHIFT_PORT PORTB
#define CFG_SHIFT_SRCLK	PB1
#define CFG_SHIFT_RCLK	PB2
#define CFG_SHIFT_SER	PB3
#define CFG_SHIFT_REGISTER_PIN_COUNT	8


void SN74HC595NoSpi_init(void) 
{
	CFG_SHIFT_DDR |= _BV(CFG_SHIFT_RCLK)
		| _BV(CFG_SHIFT_SRCLK)
		| _BV(CFG_SHIFT_SER);
}

void SN74HC595NoSpi_shift_bits_out(bits_type b, size_t len) {
	CFG_SHIFT_PORT &= ~_BV(CFG_SHIFT_RCLK);
	for (size_t i = 0; i < CFG_SHIFT_REGISTER_PIN_COUNT; i++) {
		CFG_SHIFT_PORT &= ~_BV(CFG_SHIFT_SRCLK);
		if (i < len && (b & ((bits_type)1 << i)))
			CFG_SHIFT_PORT |= _BV(CFG_SHIFT_SER);
		CFG_SHIFT_PORT |= _BV(CFG_SHIFT_SRCLK);
		CFG_SHIFT_PORT &= ~_BV(CFG_SHIFT_SER);
	}
	CFG_SHIFT_PORT |= _BV(CFG_SHIFT_RCLK);
}

int SN74HC595NoSpi_Sample(void)
{
	SN74HC595NoSpi_init();

	while (1) {
		SN74HC595NoSpi_shift_bits_out(0x00, 8);
		_delay_ms(500);
		SN74HC595NoSpi_shift_bits_out(0xff, 8);
		_delay_ms(500);
	}
	return 0;
}
