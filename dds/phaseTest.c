

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"

static inline void initTimer0(void) {

  set_bit(TCCR0A, COM0A1);                      /* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER);               /* enable output on pin */

  set_bit(TCCR0A, WGM00);                             /* Fast PWM mode */
  set_bit(TCCR0A, WGM01);                       /* Fast PWM mode, pt.2 */

  //  set_bit(TCCR0B, CS00);                /* Clock with /1 prescaler */
  //  set_bit(TCCR0B, CS01);               /* Clock with /64 prescaler */
  set_bit(TCCR0B, CS02);                  /* Clock with /256 prescaler */

  //  set_bit(TIMSK0, TOIE0);                    /* Overflow interrupt */
  //  sei();                                       /* Enable interrupt */
}

static inline void pollButton(void) {
  if (bit_is_clear(BUTTON_IN, BUTTON)) {
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

  set_bit(BUTTON_PORT, BUTTON);                    /* pullup on button */
  set_bit(LED_DDR, LED0);
  set_bit(LED_PORT, LED0);
  _delay_ms(100);
  clear_bit(LED_PORT, LED0);

  // ------ Event loop ------ //
  while (1) {

    while (i > 50) {
      i--;
      _delay_ms(20);
      OCR0A = i;
      pollButton();
    }
    while (i < 200) {
      i++;
      _delay_ms(20);
      OCR0A = i;
      pollButton();
    }


  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
