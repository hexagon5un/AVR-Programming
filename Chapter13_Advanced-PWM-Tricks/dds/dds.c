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
  uint16_t accumulatorSteps = 880;                    /* approx 440 Hz */
  uint8_t waveStep;
  int8_t pwmValue;

  // -------- Inits --------- //

  initTimer0();
  BUTTON_PORT |= (1 << BUTTON);                    /* pullup on button */

  // ------ Event loop ------ //
  while (1) {

    if (bit_is_clear(BUTTON_PIN, BUTTON)) {

      SPEAKER_DDR |= (1 << SPEAKER);                 /* enable speaker */
      accumulator += accumulatorSteps;          /* advance accumulator */
      waveStep = accumulator >> 8;           /* which entry in lookup? */
      pwmValue = fullSine[waveStep];                 /* lookup voltage */

      loop_until_bit_is_set(TIFR0, TOV0);        /* wait for PWM cycle */
      OCR0A = 128 + pwmValue;                     /* set new PWM value */
      TIFR0 |= (1 << TOV0);                  /* reset PWM overflow bit */
    }

    else {                                       /* button not pressed */
      SPEAKER_DDR &= ~(1 << SPEAKER);              /* disable speaker */
    }

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
