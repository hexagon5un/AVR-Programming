
/*
   Direct-digital synthesis
   Mixer Demo

*/

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "macros.h"
#include "fullSine.h"
#include "scale.h"

static inline void initTimer0(void) {
  set_bit(TCCR0A, COM0A1);                      /* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER);               /* enable output on pin */

  set_bit(TCCR0A, WGM00);                             /* Fast PWM mode */
  set_bit(TCCR0A, WGM01);                       /* Fast PWM mode, pt.2 */

  set_bit(TCCR0B, CS00);                    /* Clock with /1 prescaler */
}

static inline void initLEDs(void) {
  uint8_t i;
  LED_DDR = 0xff;                          /* All LEDs for diagnostics */
  for (i = 0; i < 8; i++) {
    set_bit(LED_PORT, i);
    _delay_ms(100);
    clear_bit(LED_PORT, i);
  }
}

int main(void) {

  uint16_t accumulators[4] = { 0, 0, 0, 0 };
  uint16_t tuningWords[4] = { C2, E2, G2, C3 }; // Notes are a C Maj chord
