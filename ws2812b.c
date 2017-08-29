#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "helper.h"

#define BRIGHTNESS 255

extern void output_grb_strip(uint8_t *ptr, uint16_t count);

void set_color(uint8_t *p_buf, uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t index = 3 * led;
    p_buf[index++] = g;
    p_buf[index++] = r;
    p_buf[index] = b;
}

uint8_t actual_brightness(uint8_t brightness)
{
    return (brightness * brightness) / 255;
    //return brightness;
}

int main()
{
    DDRB |= (POUT << PB5);
    DDRD |= (POUT << PD2);
    PORTB &= ~(1 << PB5);
    PORTD &= ~(1 << PD2);

    uint8_t leds[12];
    memset(leds, 0, sizeof(leds));

    set_color(leds, 0, 0, 0, 0);
    set_color(leds, 1, 0, 0, 0);
    set_color(leds, 2, 0, 0, 0);
    set_color(leds, 3, 0, 0, 0);
    output_grb_strip(leds, sizeof(leds));

    uint8_t brightness = 1;
    uint8_t add = 1;

    while(1)
    {
        uint8_t log = actual_brightness(brightness)/10;
        set_color(leds, 0, log, log, log);
        set_color(leds, 1, log, log, log);
        set_color(leds, 2, log, log, log);
        set_color(leds, 3, log, log, log);
        output_grb_strip(leds, sizeof(leds));
        _delay_ms(5);
        if(brightness >= 255 || brightness <= 0)
            add *= -1;
        brightness += add;
    }
}

