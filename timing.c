#include <avr/io.h>
#include <avr/interrupt.h>
#include "helper.h"

uint16_t eights = 0;

void init()
{
    DDRB |= (POUT << PB5);   /* Set pin 5 on port B as output */
    TCCR1B |= (1 << WGM12);  /* Set timer1 to CTC mode */
    TIMSK1 |= (1 << OCIE1A); /* Enable timer1 clear interrupt */
    sei();                   /* Enable global interrupts */
    OCR1A = 1953;            /* Set timer1 A register to reset every 1/8s */
    TCCR1B |= ((1 << CS10) | (1 << CS12)); /* Set the timer1 prescaler to 1024 */
}

int main(void)
{
    init();
    uint8_t previous = eights;

    while(1)
    {
        if(eights != previous && eights % 8 == 0) /* This runs every second */
        {
            previous = eights;
            PORTB ^= (1 << PB5); /* Switch the state of the LED (1 to 0, 0 to 1) */
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    if(eights == 65535) /* Loop our eights counter back to 0 so we don't overflow */
        eights = 0;
    else
        eights++;
}