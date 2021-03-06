#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "helper.h"

#define BAUD 9600
#define MORSE_UNIT 250
#define MORSE_DOT MORSE_UNIT
#define MORSE_DASH (3*MORSE_UNIT)
#define MORSE_SYMBOL MORSE_UNIT
#define MORSE_LETTER (3*MORSE_UNIT)
#define MORSE_WORD (7*MORSE_UNIT)

#include <util/setbaud.h>

volatile uint8_t buffer[64];
volatile uint8_t buffer_top;
volatile uint8_t run;

void usart_init(void)
{
    UBRR0H = UBRRH_VALUE; /* Set high bits of baud rate */
    UBRR0L = UBRRL_VALUE; /* Set low bits of baud rate */

#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~((1 << U2X0));
#endif

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8-bit data */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);   /* Enable RX and TX */
    UCSR0B |= (1 << RXCIE0); /* Enable RX interrupts */
    sei();  /* Enable global interrupts */
}

void init()
{
    DDRB |= (POUT << PB5); /* Set the 5th pin on port B to output */
}

void morse_display(uint8_t letter)
{
    /* The length of this letter is in the last 3 bits, hence the bitwise and 0x07 */
    for(int i = 0; i < (letter & 0x07); ++i)
    {
        PORTB |= (1 << PB5); /* Turn on the LED */
        /* If the 7-i bit is one, than it's a 'dash', if not, it's a 'dot' */
        _delay_ms((letter & (1 << (7 - i))) > 0 ? MORSE_DASH : MORSE_DOT);
        PORTB &= ~(1 << PB5); /* Turn off the LED */
        _delay_ms(MORSE_SYMBOL); /* Delay between a single symbol (dot or dash) */
    }
    _delay_ms(MORSE_LETTER); /* Delay between letters */
}

/**
 *
 * @param letter - an uppercase latin letter or a digit
 * @return a packed 8-bit integer containing the symbols and length of a letter (ex. 'S' = 0b00000 011)
 */
uint8_t morse_code(uint8_t letter)
{
    switch(letter)
    {
        case '0':
            return 0xFD;
        case '1':
            return 0x7D;
        case '2':
            return 0x3D;
        case '3':
            return 0x1D;
        case '4':
            return 0x0D;
        case '5':
            return 0x05;
        case '6':
            return 0x85;
        case '7':
            return 0xC5;
        case '8':
            return 0xE5;
        case '9':
            return 0xF5;
        case 'A':
            return 0x42;
        case 'B':
            return 0x84;
        case 'C':
            return 0xA4;
        case 'D':
            return 0x83;
        case 'E':
            return 0x01;
        case 'F':
            return 0x24;
        case 'G':
            return 0xC3;
        case 'H':
            return 0x04;
        case 'I':
            return 0x02;
        case 'J':
            return 0x74;
        case 'K':
            return 0xA3;
        case 'L':
            return 0x44;
        case 'M':
            return 0xC2;
        case 'N':
            return 0x82;
        case 'O':
            return 0xE3;
        case 'P':
            return 0x64;
        case 'Q':
            return 0xD4;
        case 'R':
            return 0x43;
        case 'S':
            return 0x03;
        case 'T':
            return 0x81;
        case 'U':
            return 0x23;
        case 'V':
            return 0x14;
        case 'W':
            return 0x63;
        case 'X':
            return 0x94;
        case 'Y':
            return 0xB4;
        case 'Z':
            return 0xC4;
        default:
            return 0x00;
    }
}

int main()
{
    init();
    usart_init();

    run = 0;
    buffer_top = 0;

    while(1)
    {
        if(run) /* When an LF or CR is received we start displaying the string from a buffer */
        {
            for(int i = 0; i < buffer_top; ++i) /* Loop through every letter in the buffer */
            {
                if(buffer[i] == ' ')
                {
                    _delay_ms(MORSE_WORD); /* Pause between words */
                }
                else
                {
                    morse_display(morse_code(buffer[i])); /* Fetch the packed int and display it using 'morse_display' */
                }
            }
            run = 0; /* We have finished displaying the morse code*/
            buffer_top = 0;
        }
    }
}

ISR(USART_RX_vect)
{
    uint8_t val = UDR0;
    if(val == '\x0a' || val == '\x0d') /* 0x0A - LF, 0x0D - CR */
    {
        run = 1;
    }
    else if(!run && buffer_top < sizeof(buffer)) /* If we are running ignore the inputs */
    {
        if((val >= 'A' && val <= 'Z') || (val >= '0' && val <= '9'))
        {
            buffer[buffer_top] = val;
            buffer_top++; /* Increase the top so we know how many chars to display  */
        }
        else if((val >= 'a') && (val <= 'z'))
        {
            buffer[buffer_top] = val - 32; /* Shift the lowercase letters by 32 to be the same as uppercase  */
            buffer_top++; /* Increase the top so we know how many chars to display  */
        }
    }
}