                                                         /* Cylon Eyes */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

#define DELAYTIME 45                                   /* milliseconds */
#define LED_PORT                PORTB
#define LED_PIN                 PINB
#define LED_DDR                 DDRB

int main(void) {

  // -------- Inits --------- //
  uint16_t x = 0x1234;
  uint8_t y;
  LED_DDR = 0xff;               /* Data Direction Register B:
                                   all set up for output */

  // ------ Event loop ------ //
  while (1) {

    x = 2053 * x + 13849;                 /* "random" number generator */
    y = (x >> 8) & 0b00000111;       /* pick three bits from high byte */
    LED_PORT ^= (1 << y);                            /* toggle one bit */
    _delay_ms(100);

  }                                                  /* End event loop */
  return 0;
}
