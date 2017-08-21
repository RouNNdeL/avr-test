#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600

#include <util/setbaud.h>

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
    UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0); /* Enable RX and TX interrupts */
    sei();  /* Enable global interrupts */
}

int main()
{
    usart_init();
    while(1);
}

void usart_transmit(uint8_t data)
{
    while(!(UCSR0A & (1 << UDRE0))); /* Wait for the transmit register to be free */
    UDR0 = data;
}

uint8_t usart_receive()
{
    while(!(UCSR0A) & (1 << RXC0)); /* Wait while data is being received */
    return UDR0;
}

ISR(USART_RX_vect)
{
    //On receive finished interrupt
}

ISR(USART_TX_vect)
{
    //On send finished interrupt
}