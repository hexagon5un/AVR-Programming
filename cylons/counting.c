#include <avr/io.h>                        /* Defines pins, ports, etc */
#define F_CPU 1000000UL          /* Sets up the chip speed for delay.h */
#include <util/delay.h>                     /* Functions to waste time */

#define DELAYTIME 100

void main(void) {
  DDRB = 0b11111111;
  PORTB = 0;

  while (1) {
    PORTB = PORTB + 1;
    _delay_ms(DELAYTIME);
  }
}
