                                           /* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

#include "pinDefines.h"
#include "macros.h"
#include "fullSine.h"

static inline void initTimer0(void) {
  set_bit(TCCR0A, COM0A1);                      /* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER);               /* enable output on pin */

  set_bit(TCCR0A, WGM00);                             /* Fast PWM mode */
  set_bit(TCCR0A, WGM01);                       /* Fast PWM mode, pt.2 */

  set_bit(TCCR0B, CS00);                    /* Clock with /1 prescaler */
}

int main(void) {

  uint16_t accumulator;
  uint16_t accumulatorSteps;
  uint8_t waveStep;
  int8_t pwmValue;
  uint16_t i;

  // -------- Inits --------- //

  initTimer0();
  set_bit(BUTTON_PORT, BUTTON);                    /* pullup on button */

  // ------ Event loop ------ //
  while (1) {

    if (bit_is_clear(BUTTON_PIN, BUTTON)) {

      set_bit(SPEAKER_DDR, SPEAKER);             /* set speaker output */

      accumulator += accumulatorSteps;          /* advance accumulator */
      waveStep = accumulator >> 8;            /* which step are we on? */
      pwmValue = fullSine[waveStep];                 /* lookup voltage */

      loop_until_bit_is_set(TIFR0, TOV0);     /* wait for overflow bit */
      OCR0A = 128 + pwmValue;                        /* Set PWM output */
      set_bit(TIFR0, TOV0);                /* reset timer overflow bit */

      if (accumulator < accumulatorSteps) {          /* once per cycle */
        if (accumulatorSteps > 100) {
          accumulatorSteps = accumulatorSteps - 3;  /* lower the pitch */
        }
      }
    }
    else {                                       /* button not pressed */
      clear_bit(SPEAKER_DDR, SPEAKER);           /* disconnect speaker */
      accumulatorSteps = 1600;                          /* reset pitch */
    }


  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
