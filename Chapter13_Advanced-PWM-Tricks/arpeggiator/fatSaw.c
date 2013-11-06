/*
   Direct-digital synthesis
   Phasing saw waves demo

*/

#include "fatSaw.h"

int main(void) {

  uint16_t accumulators[NUMBER_OSCILLATORS];
  uint8_t waveStep;
  int16_t mixer;
  uint8_t i;

  // -------- Inits --------- //

  initTimer0();
  SPEAKER_DDR |= (1 << SPEAKER);                     /* speaker output */
  LED_DDR |= (1 << LED0);

  // Init all to same phase
  for (i = 0; i < NUMBER_OSCILLATORS; i++) {
    accumulators[i] = 0;
  }

  // ------ Event loop ------ //
  while (1) {

                                   /* Load in the PWM value when ready */
    loop_until_bit_is_set(TIFR0, TOV0); /* wait until overflow bit set */
    OCR0A = 128 + mixer;           /* signed-integers need shifting up */
    TIFR0 |= (1 << TOV0);                   /* re-set the overflow bit */

                              /* Update all accumulators, mix together */
    mixer = 0;
    for (i = 0; i < NUMBER_OSCILLATORS; i++) {
      accumulators[i] += BASEPITCH;
      waveStep = accumulators[i] >> 8;

      // Add extra phase increment.
      // Makes shifting overtones when
      // different frequency components add, subtract
      if (waveStep == 0) {                   /* roughly once per cycle */
        accumulators[i] += PHASE_RATE * i;          /* add extra phase */
      }

      mixer += fullSaw15[waveStep];
    }
    mixer = mixer >> OSCILLATOR_SHIFT;
                                 /* Dividing by bitshift is very fast. */

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
