                                           /* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

#include "pinDefines.h"

#include "fullSine.h"

static inline void initTimer0(void) {
  TCCR0A |= (1 << COM0A1);                      /* PWM output on OCR0A */
  SPEAKER_DDR |= (1 << SPEAKER);               /* enable output on pin */

  TCCR0A |= (1 << WGM00);                             /* Fast PWM mode */
  TCCR0A |= (1 << WGM01);                       /* Fast PWM mode, pt.2 */

  TCCR0B |= (1 << CS00);                    /* Clock with /1 prescaler */
}

int main(void) {

  uint16_t accumulator;
  uint16_t accumulatorSteps;
  uint8_t waveStep;
  int8_t pwmValue;
  uint16_t i;

  // -------- Inits --------- //

  initTimer0();
  BUTTON_PORT |= (1 << BUTTON);                    /* pullup on button */

  // ------ Event loop ------ //
  while (1) {

    if (bit_is_clear(BUTTON_PIN, BUTTON)) {

      SPEAKER_DDR |= (1 << SPEAKER);             /* set speaker output */

      accumulator += accumulatorSteps;          /* advance accumulator */
      waveStep = accumulator >> 8;            /* which step are we on? */
      pwmValue = fullSine[waveStep];                 /* lookup voltage */

      loop_until_bit_is_set(TIFR0, TOV0);     /* wait for overflow bit */
      OCR0A = 128 + pwmValue;                        /* Set PWM output */
      TIFR0 |= (1 << TOV0);                /* reset timer overflow bit */

      if (accumulator < accumulatorSteps) {          /* once per cycle */
        if (accumulatorSteps > 100) {
          accumulatorSteps = accumulatorSteps - 3;  /* lower the pitch */
        }
      }
    }
    else {                                       /* button not pressed */
      SPEAKER_DDR &= ~(1 << SPEAKER);           /* disconnect speaker */
      accumulatorSteps = 1600;                          /* reset pitch */
    }


  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
