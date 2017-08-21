#include <avr/eeprom.h>
#include <avr/io.h>
#include "helper.h"

#define ADDR 0x10 /* Define our address in EEPROM for testing */

void init()
{
    DDRB |= (POUT << PB5); /* Set pin 5 on port B to output */
    PORTB &= ~(1 << PB5);  /* Set pn 5 on port B to 0 */
}

int main()
{
    init();

    eeprom_update_byte((uint8_t *) ADDR, 123); /* Write 123 to address ADDR in EEPROM */

    uint8_t byte;
    byte = eeprom_read_byte((uint8_t *) ADDR); /* Read value at ADDR from EEPROM */

    if(byte == 123)
    {
        PORTB |= (1 << PB5);  /* If the values match turn on the LED */
    }

    while(1);
}