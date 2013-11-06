
/*
   Direct-digital synthesis
   Mixer Demo

*/

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include <avr/interrupt.h>
#include "pinDefines.h"

#include "fullSine.h"
#include "scale.h"

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

  uint16_t accumulators[4] = { 0, 0, 0, 0 };
  uint16_t tuningWords[4] = { C2, E2, G2, C3 }; // Notes are a C Maj chord
