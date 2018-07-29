#ifndef F_CPU
#define F_CPU 20000000UL	// or whatever may be your frequency
#endif

#include <avr/io.h>
#include <avr/delay.h>
#include <math.h>
#include <avr/interrupt.h>

// inverted or outverted.

void PwmSamples_TIMER0_FastPwmMode();
void PwmSamples_TIMER0_PhaseCorrectPwmMode();

void PwmSamples_TIMER1_FastPwmMode();
void PwmSamples_TIMER1_PhaseCorrectPwmMode();
void PwmSamples_TIMER1_PhaseAndFrequencyCorrectPwmMode();

void PwmSamples_TIMER2_FastPwmMode();
void PwmSamples_TIMER2_PhaseCorrectPwmMode();


void PwmSamples_SampleOne();
// TODO: Ensure you use PWM port ~
void PwmSamples_SampleOne()
{
	DDRD |= (1 << PD6);    // Set our pwm pin as an output

	// Set it to low, just to be safe
	PORTD &= ~(1 << PD6);

	// Timer configuration

	TCCR0A = (1 << COM0A1);	// Clear OC0A on Compare Match.
	TCCR0A = (1 << WGM01) | (1 << WGM00);	// Enable fast pwm mode by setting WGM bitsS
	TCCR0B = ((1 << CS01) | (1 << CS00));   // Set prescaler to 64

	for (;;) 
	{
		//Fade up
		for (unsigned char i = 0; i<255;i++)
		{
			OCR0A = i;        //Set new duty cycle value
			_delay_ms(50);    //delay a litle bit
		}

		//Fade down
		for (unsigned char i = 255; i>0;i--)
		{
			OCR0A = i;        //Set new duty cycle value
			_delay_ms(50);    //delay a litle bit
		}
	}
}

#define INTENSITY    0    //The pot that controll the intensity is attached to adc pin 0, aka PC0
#define TIME         1     //The delay time is controlled by the pot attached to adc pin 1, aka PC1

//Functions prototypes
void timer0_init(void);
void timer1_init(void);
void timer2_init(void);
void adc_init(void);
void set_pwm(uint8_t red, uint8_t green, uint8_t blue);
void Hsv2Rgb(double *r, double *g, double *b, double h, double s, double v);
unsigned long PwmSamples_Milis(void);
uint16_t read_adc(uint8_t channel);

//Global Variables
volatile unsigned long milis_count = 0;    //This var is shared between function and an ISR, so it must be declared volatile

void PwmSamples_SampleTwo();

void PwmSamples_SampleTwo()
{
	//Local variables declaration
	unsigned int delayTime = 10;       //Variable to store the value read from one of the pots
	uint16_t intensity = 0;            //Variable to store the value read from the other pot
	unsigned long lastTime = 0;        //Variable to count how much time as passed
	double red = 0.0;                  //Actual red value
	double green = 0.0;                //Actual green value
	double blue = 0.0;                 //Actual blue value
	double hue = 0.0;                  //Actual hue value, this says what color is it
	double saturation = 100;           //Set saturation value to maximum, this value can be tweaked
	double value = 1.0;                //This is where we set the light intensity using the pot

	timer0_init();            //Setup timer 0
	timer1_init();            //Setup timer 1
	timer2_init();            //Setup timer 2
	adc_init();               //Setup the ADC

	sei();        //Enable global interrupts so our milis function can work
	DDRB = (1 << PB5);

	for (;;) 
	{
		if ((PwmSamples_Milis() - lastTime) >= delayTime) 
		{
			PORTB ^= (1 << PB5);
			lastTime = PwmSamples_Milis();

			//Read both the time and intensity pots
			delayTime = read_adc(TIME) * 2;    //I use times two here to set the max delay to 2 seconds, but you can tweak the code
											   //to increase or decrease this

			if (delayTime <= 10) {
				delayTime = 10;                //Just to make sure that we always wait at least 10ms to change color;
			}

			intensity = read_adc(INTENSITY);
			value = ((double)(intensity / 1024.0)*255.0);    //This is like a map, so the intensity value is between 0 and 255

			if (value >= 255.0) {
				value = 255.0;    //Just to make sure that we don't go beyond 255 or it can cause strange results
			}

			if (value <= 0.1) {
				value = 0.0;
			}

			//Update the duty-cycles to output the next color

			Hsv2Rgb(&red, &green, &blue, hue, saturation, value);

			red = red * 255.0;            //This is needed be cause the conversion formula returns values between 0 and 1
			green = green * 255.0;        //And we need values between 0 and 255 for our pwm generators
			blue = blue * 255.0;

			hue = hue + 1.0;        //You can also tweak this value to have bigger or smaller steps between colors

			if (hue > 360) {
				hue = 0;
			}

			OCR0A = (uint8_t)red;    //Red led is connected to pin PD6/digital 6
			OCR0B = (uint8_t)green;  //Green led is connected to pin PD5/digital 5
			OCR2B = (uint8_t)blue;   //Blue led is connected to pin PD3/digital pin 3

									 //If you are using an RGB led this will work out of the box for common anode ones
		}
	}
}

void adc_init(void) 
{
	ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (1 << REFS0);                             //Voltage reference from Avcc (5v)
	ADCSRA |= (1 << ADEN);                             //Turn on ADC
	ADCSRA |= (1 << ADSC);                             //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}

uint16_t read_adc(uint8_t channel) 
{
	ADMUX &= 0xF0;                   //Clear the older channel that was read
	ADMUX |= channel;                //Defines the new ADC channel to be read
	ADCSRA |= (1 << ADSC);             //Starts a new convertion
	while (ADCSRA & (1 << ADSC));       //Wait until the convertion is done
	return ADCW;                     //Returns the ADC value of the choosen channel
}

void timer0_init(void) 
{
	DDRD = ((1 << PD5) | (1 << PD6));                      //Set pwm pins from timer 0 as outputs
	TCCR0A = ((1 << COM0A1) | (1 << WGM01) | (1 << WGM00));    //Enable pwm mode in pin PD6 and set the WGM bits to Fast pwm mode
	TCCR0A |= (1 << COM0B1);                           //Enable pwm mode in pin PD5
	TCCR0B = ((1 << CS01) | (1 << CS00));                  //Set prescaler to 32???64
}

void timer2_init(void) 
{
	DDRD |= (1 << PD3);                                //Set pwm pin OC2B from timer 2 as output
	TCCR2A = ((1 << COM2B1) | (1 << WGM21) | (1 << WGM20));    //Enable pwm mode in PD3 and set the WGM bits for fast pwm mode
	TCCR2B = ((1 << CS21) | (1 << CS20));                  //Set prescaler to 32
														   //As you can see, the code for timer 0 and 2 is very similiar, and even almost registers are identical, this is equal also for timer 1
}

void timer1_init(void) 
{
	TCCR1B = ((1 << WGM12) | (1 << CS10));    //Timer in CTC mode, prescaler set to 1
	OCR1A = 15999;                      //Our target count to have an 1ms interrupt
	TIMSK1 = (1 << OCIE1A);               //Enable interrupt when OCR1A value is reached
}

unsigned long PwmSamples_Milis(void) 
{
	cli();   //Disable all interrupts so we can read our long variable atomically
			 //This is to ensure that no interrupt is fired while reading the long variable
			 //And possibly trash the readed value
	unsigned long milis_value = milis_count;    //Copy the value and return it
	sei();        //Enable all interrupt again
	return milis_value;
}

ISR(TIMER1_COMPA_vect) 
{
	milis_count++;    //Just add 1 to our milis value
}

void Hsv2Rgb(double *r, double *g, double *b, double h, double s, double v) 
{
	int i;
	double f, p, q, t;

	s /= 100.0;
	v /= 255.0;

	if (s == 0) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= 60;            // sector 0 to 5
	i = (int)floor(h);
	f = h - i;            // factorial part of h
	p = v * (1.0 - s);
	q = v * (1.0 - s * f);
	t = v * (1.0 - s * (1 - f));

	switch (i) {
	case 0:
		*r = v;
		*g = t;
		*b = p;
		break;
	case 1:
		*r = q;
		*g = v;
		*b = p;
		break;
	case 2:
		*r = p;
		*g = v;
		*b = t;
		break;
	case 3:
		*r = p;
		*g = q;
		*b = v;
		break;
	case 4:
		*r = t;
		*g = p;
		*b = v;
		break;
	default:        // case 5:
		*r = v;
		*g = p;
		*b = q;
		break;
	}
}