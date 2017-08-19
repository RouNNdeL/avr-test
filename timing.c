#include <avr/io.h>
#include <avr/interrupt.h>
#include "helper.h"

uint16_t eights = 0;

void init()
{
    DDRB |= (POUT << PB5);
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);
    sei();
    OCR1A = 1953;
    TCCR1B |= ((1 << CS10) | (1 << CS12));
}

int main(void)
{
    init();
    uint8_t previous = eights;

    while (1)
    {
        if (eights != previous && eights % 8 == 0)
        {
            PORTB ^= (1 << PB5);
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (eights == 65528)
        eights = 0;
    else
        eights++;
}