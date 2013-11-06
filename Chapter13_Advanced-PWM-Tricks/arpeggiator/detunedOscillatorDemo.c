
/*
   Direct-digital synthesis
   Mixer Demo

*/

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include <avr/interrupt.h>
#include "pinDefines.h"

#include "fullTriangle.h"

#define BASEPITCH       800     /* in tuningWord steps, roughly 1/2 Hz */
#define INITIAL_DETUNE  64      /* how many updates until all together */
#define SPEED_FACTOR    8    /* 1,2,4,8 are good. Odd values not good. */

static inline void initTimer0(void) {
  TCCR0A |= (1 << COM0A1);                      /* PWM output on OCR0A */
  SPEAKER_DDR |= (1 << SPEAKER);               /* enable output on pin */

  TCCR0A |= (1 << WGM00);                             /* Fast PWM mode */
  TCCR0A |= (1 << WGM01);                       /* Fast PWM mode, pt.2 */

  TCCR0B |= (1 << CS00);                    /* Clock with /1 prescaler */
}


static inline void initLEDs(void) {
  uint8_t i;
  LED_DDR = 0xff;                          /* All LEDs for diagnostics */
  for (i = 0; i < 8; i++) {
    LED_PORT |= (1 << i);
    _delay_ms(100);
    LED_PORT &= ~(1 << i);
  }
}

int main(void) {

  uint16_t accumulators[8];
  uint16_t tuningWords[8];
  uint16_t mixer;
  uint8_t i;
  uint16_t cycles;
  uint8_t counter = 0;

  // -------- Inits --------- //

  initLEDs();
  initTimer0();

  BUTTON_PORT |= (1 << BUTTON);                    /* pullup on button */
  SPEAKER_DDR |= (1 << SPEAKER);                     /* speaker output */

  // Start off each on a different pitch, but in phase
  for (i = 0; i < 8; i++) {
    tuningWords[i] = BASEPITCH - INITIAL_DETUNE * i;
    accumulators[i] = 0;
  }


  // ------ Event loop ------ //
  while (1) {

    loop_until_bit_is_set(TIFR0, TOV0); /* wait until overflow bit set */
    TIFR0 |= (1 << TOV0);               /* writing set should reset... */
    OCR0A = 128 + mixer;

                              /* Update all accumulators, mix together */
    mixer = 0;
    for (i = 0; i < 8; i++) {
      accumulators[i] += tuningWords[i];
      mixer += fullTriangle[(uint8_t) (accumulators[i] >> 8)];  // triangle version
