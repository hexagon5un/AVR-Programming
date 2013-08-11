/*
Plays a simple tune, broadcasts it in the AM radio band.
*/

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"
#include "scale16.h"

#define COUNTER_VALUE   5              /* determines carrier frequency */

// From f = f_cpu / ( 2* N* (1 + OCRnx) )
// Good values for the AM band from 2 to 6: pick one that's clear
// Divide by two b/c we're toggling on or off each loop;
//  a full cycle of the carrier takes two loops.
// 8Mhz / (2 * 1 * (1+2)) = 1333 kHz
// 8Mhz / (2 * 1 * (1+3)) = 1000 kHz
// 8Mhz / (2 * 1 * (1+4)) = 800 kHz
// 8Mhz / (2 * 1 * (1+5)) = 670 kHz
// 8Mhz / (2 * 1 * (1+6)) = 570 kHz
// 8Mhz / (2 * 1 * (1+7)) = 500 kHz

#define NOTE_LENGTH  50000

static inline void initTimer(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0A |= (1 << COM0B0);            /* Toggles pin each time through */
  TCCR0B |= (1 << CS00);              /* Clock at CPU frequency, ~8MHz */
  OCR0A = COUNTER_VALUE;                          /* carrier frequency */
}

static inline void transmitBeep(uint16_t pitch) {
  uint16_t elapsed;
  uint16_t i;
  for (elapsed = 0; elapsed < NOTE_LENGTH; elapsed += pitch) {
    for (i = 0; i < pitch; i++) {
      _delay_us(1);                          /* delay for pitch cycles */
    }
    ANTENNA_DDR ^= (1 << ANTENNA);        /* toggle carrier on and off */
  }
  ANTENNA_DDR |= (1 << ANTENNA);               /* back on full carrier */
}

int main(void) {

  uint8_t i;

  // -------- Inits --------- //
  initTimer();

  // ------ Event loop ------ //
  while (1) {

    transmitBeep(E3);
    _delay_ms(50);
    transmitBeep(E3);
    _delay_ms(200);
    transmitBeep(E3);
    _delay_ms(200);
    transmitBeep(C3);
    transmitBeep(C3);
    transmitBeep(E3);
    _delay_ms(200);
    transmitBeep(G3);
    transmitBeep(G3);
    transmitBeep(G3);
    transmitBeep(G3);
    _delay_ms(400);
    transmitBeep(G2);
    transmitBeep(G2);
    transmitBeep(G2);
    transmitBeep(G2);

    _delay_ms(2500);

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
