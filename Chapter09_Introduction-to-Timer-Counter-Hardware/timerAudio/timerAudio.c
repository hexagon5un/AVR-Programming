/*

Quick audio demo using Timer 0 to generate audio frequencies directly.

*/

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"
#include "scale8.h"                                     /* 8-bit scale */

static inline void initTimer(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0A |= (1 << COM0A0);           /* Toggles pin each cycle through */
  TCCR0B |= (1 << CS00) | (1 << CS01);               /* CPU clock / 64 */
}

static inline void playNote(uint8_t wavelength, uint16_t duration) {

  TCNT0 = 0;                                      /* reset the counter */
  OCR0A = wavelength;                                     /* set pitch */
  SPEAKER_DDR |= (1 << SPEAKER);           /* enable output on speaker */

  while (duration) {                                 /* Variable delay */
    _delay_ms(1);
    duration--;
  }
  SPEAKER_DDR &= ~(1 << SPEAKER);                  /* turn speaker off */
}

int main(void) {
  // -------- Inits --------- //
  initTimer();
  // ------ Event loop ------ //
  while (1) {
                                                    /* Play some notes */
    playNote(C2, 200);
    playNote(E2, 200);
    playNote(G2, 200);
    playNote(C3, 400);

    _delay_ms(1000);
    _delay_ms(1000);
    _delay_ms(1000);

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
