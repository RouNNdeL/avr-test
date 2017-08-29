#include <avr/io.h>
#include <avr/delay.h>

void pwm_init()
{
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
    DDRD |= (1 << PD3);
    //PORTB &= ~(1 << PB1) & ~(1 << PB2) & ~(1 << PB3);
    //PORTB |= (1 << PB2) | (1 << PB1);

    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
    TCCR1B = _BV(CS12);
    TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
    TCCR2B = _BV(CS22);

    OCR1A = 0;   //BLUE
    OCR1B = 0;
    OCR2A = 100; //GREEN
    OCR2B = 0;   //RED
}

uint8_t actual_brightness(uint8_t brightness)
{
    return (brightness * brightness) / 255;
    //return brightness;
}

int main()
{
    pwm_init();

    uint8_t brightness = 1;
    uint8_t add = 1;
    uint8_t color = 0;

    while(1)
    {
        _delay_ms(5);
        if(brightness >= 255)
        {
            add *= -1;
        }
        else if(brightness <= 0)
        {
            add *= -1;
            color = (color+1)%3;
        }
        brightness += add;
        if(color == 0)
        {
            OCR2B = actual_brightness(brightness);
        }
        else if(color == 1)
        {
            OCR1A = actual_brightness(brightness);
        }
        else if(color == 2)
        {
            OCR2A = actual_brightness(brightness);
        }
    }
}