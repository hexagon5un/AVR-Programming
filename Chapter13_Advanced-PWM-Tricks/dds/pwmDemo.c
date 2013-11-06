/* PWM Demo, meant for LED.
   Brightens / dims.  Woots.
 */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"


static inline void initTimer0(void) {

  TCCR0A |= (1 << COM0A1);                      /* PWM output on OCR0A */

  TCCR0A |= (1 << WGM00);                             /* Fast PWM mode */
  TCCR0A |= (1 << WGM01);                       /* Fast PWM mode, pt.2 */

  TCCR0B |= (1 << CS02);                  /* Clock with /256 prescaler */
}

static inline void pollButton(void) {
  if (bit_is_clear(BUTTON_PIN, BUTTON)) {
    SPEAKER_DDR |= (1 << SPEAKER);             /* enable output on pin */
    LED_PORT &= ~(1 << LED0);
  }
  else {
    SPEAKER_DDR &= ~(1 << SPEAKER);          /* disable output on pin */
    LED_PORT |= (1 << LED0);
  }
}

int main(void) {
  uint8_t i;

  // -------- Inits --------- //

  initTimer0();

  SPEAKER_DDR |= (1 << SPEAKER);   /* enable output on SPEAKER / OCR0A */
  BUTTON_PORT |= (1 << BUTTON);                    /* pullup on button */

  LED_DDR |= (1 << LED0);                         /* enable LED output */
  LED_PORT |= (1 << LED0);                                 /* test LED */
  _delay_ms(100);
  LED_PORT &= ~(1 << LED0);

  // ------ Event loop ------ //
  while (1) {

    // Increase
    while (i < 255) {
      i++;
      _delay_ms(2);
      OCR0A = i;
      pollButton();
    }
    // Decrease
    while (i > 0) {
      i--;
      _delay_ms(2);
      OCR0A = i;
      pollButton();
    }


  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
