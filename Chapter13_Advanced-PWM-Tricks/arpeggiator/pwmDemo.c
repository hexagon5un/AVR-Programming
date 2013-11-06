/* PWM Demo, meant for LED.
   Brightens / dims.  Woots.
 */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"

static inline void initTimer0(void) {

  set_bit(TCCR0A, COM0A1);                      /* PWM output on OCR0A */

  set_bit(TCCR0A, WGM00);                             /* Fast PWM mode */
  set_bit(TCCR0A, WGM01);                       /* Fast PWM mode, pt.2 */

  set_bit(TCCR0B, CS02);                  /* Clock with /256 prescaler */
}

static inline void pollButton(void) {
  if (bit_is_clear(BUTTON_PIN, BUTTON)) {
    set_bit(SPEAKER_DDR, SPEAKER);             /* enable output on pin */
    clear_bit(LED_PORT, LED0);
  }
  else {
    clear_bit(SPEAKER_DDR, SPEAKER);          /* disable output on pin */
    set_bit(LED_PORT, LED0);
  }
}

int main(void) {
  uint8_t i;

  // -------- Inits --------- //

  initTimer0();

  set_bit(SPEAKER_DDR, SPEAKER);   /* enable output on SPEAKER / OCR0A */
  set_bit(BUTTON_PORT, BUTTON);                    /* pullup on button */

  set_bit(LED_DDR, LED0);                         /* enable LED output */
  set_bit(LED_PORT, LED0);                                 /* test LED */
  _delay_ms(100);
  clear_bit(LED_PORT, LED0);

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
