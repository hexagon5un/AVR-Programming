/* Cylon Eyes
   Half-stepping turns the next light on for a little bit,
   makes the pattern visually smoother, improves overall
   cylonity.
*/

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

#define DELAYTIME 85    /* milliseconds, 50-75 is good for smooth eyes */

int main(void) {

  // -------- Inits --------- //
  uint8_t i;
  DDRB = 0xff;                  /* Data Direction Register B:
                                   all set up for output */

  // ------ Event loop ------ //
  while (1) {                                              /* mainloop */

    for (i = 0; i < 7; i++) {                   /* count i from 0 to 6 */
      PORTB |= (1 << i);                              /* turn on led i */
      _delay_ms(DELAYTIME);                                    /* wait */

      PORTB |= (1 << (i + 1));                 /* turn the next led on */
      _delay_ms(DELAYTIME / 2);

      PORTB &= ~(1 << i);                            /* turn off led i */
    }

    for (i = 7; i > 0; i--) {                   /* count i from 7 to 1 */
      PORTB |= (1 << i);
      _delay_ms(DELAYTIME);

      PORTB |= (1 << (i - 1));                 /* turn the next led on */
      _delay_ms(DELAYTIME / 2);

      PORTB &= ~(1 << i);                            /* turn off led i */
    }

  }                                                  /* End event loop */
  return (0);
}                                                          /* end main */
