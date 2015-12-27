
#define F_CPU 20000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>
#include <stdbool.h>

const uint16_t wavedelay = 2000 / 0xff;
volatile int8_t direction = 1;
volatile uint8_t comparevalue = 0;

void setupTimer()
{
    // clear output port.
    DDRC = 0xFF;
    PORTC = 0xFF;
    // enable timer0 and enable compare interrupt.
    TCCR0 |= (1 << CS00);
    TIMSK |= (1 << OCIE0);
    OCR0 = 0xFF;
    // enable interrupts.
    sei();
}

void adcInit()
{
    ADMUX = (1 << REFS1) | (1 << REFS0);
    ADCSRA = (1 << ADSC) | (1 << ADATE) | (1 << ADEN) | (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);
    SFIOR = (1 << ADTS0);
}

uint8_t adc_read(uint8_t ch)
{
    ch &= 0x07;
    ADMUX = (ADMUX & 0xF8) | ch;
    if(! (ADCSRA & (1 << ADSC)))
    {
        ADCSRA |= (1 << ADSC);
        PORTC = ADC;
        return PORTC;
    }
    else
    {
        return 0;
    }
}

void main(void)
{
    // setupTimer();
    adcInit();
    while(true)
    {
        adc_read(0);
        adc_read(1);
    }
}

ISR(TIMER0_COMP_vect)
{
    TCNT0 = 0xFF - comparevalue;
    register uint8_t r = PORTC;
    r += direction;
    if(!r)
    {
        direction = 1;
    }
    else if(r == 0xff)
    {
        direction = -1;
    }
    PORTC = rand() % 0xFF;
}