
NOTE: These samples were tested on ATMEGA328p unless stated otherwise.

http://www.atmel.com/webdoc/avrlibcreferencemanual/FAQ_1faq_port_pass.html

#### Next

page 105

create schema sample for each example
page 75 the image preview 5-7 needs to be implemented
http://maxembedded.com/2012/01/avr-timers-pwm-mode-part-ii/
https://sites.google.com/site/qeewiki/books/avr-guide
http://www.atmel.com/webdoc/avrlibcreferencemanual/group__util__delay.html

motor speed control with PWM
chapter 6 page 81 reimplement that Keyboard.begin(); with USB read write. and create calculator with buttons. then use the small joystic and use it as mouse. review examples for chapter 6
https://en.wikipedia.org/wiki/Boolean_algebra
ButtonSamples.h
button debounce
review arduion samples 4 and create C code samples
shift register add a sample for GE output enable / disable
see bookmarks for ShiftRegister folder
find icp.h and icp.c for atmega328p
find all F_CPU = 20000000 and replace with 16000000 or ensure that 2000000 is working correctly
ICP - check TIMER1_CAPT interupt, TIFR1 register, ICES1 bit

timer
pwm

atomic.h (this might be usable for locking access)

#### Reading

page 35
2.3 Electrical Characteristics of the I/O Line

#### Ohm law

R (resistance) = V (voltage) / I (current)
V = R * I
I = V / R

#### Resources

http://avrbeginners.net/
https://learn.sparkfun.com/tutorials/how-to-read-a-schematic
http://www.atmel.com/webdoc/avrlibcreferencemanual/index.html
http://www.ohmslawcalculator.com/led-resistor-calculator

Shift register sample
http://www.ermicro.com/blog/?p=1050
https://protostack.com.au/2010/05/introduction-to-74hc595-shift-register-controlling-16-leds/

#Serial Communication
http://maxembedded.com/2013/09/serial-communication-introduction/

#SPI
http://www.gammon.com.au/forum/?id=10892
https://sites.google.com/site/qeewiki/books/avr-guide/spi
http://www.rocketnumbernine.com/2009/04/26/using-spi-on-an-avr-1
https://www.tablix.org/~avian/blog/archives/2012/06/spi_interrupts_versus_polling/

#Bit_banging
https://en.wikipedia.org/wiki/Bit_banging