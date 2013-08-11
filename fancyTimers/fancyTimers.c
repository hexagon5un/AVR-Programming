/* Demo of the power of system ticks
 * Eight LEDs blink, each at just-slightly different frequencies
 * Creates a fancy phasing pattern  */

// ------- Preamble -------- //
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pinDefines.h"

// -------- Global Variables --------- //
volatile uint8_t milliseconds = 0;

// -------- Functions --------- //
void initTimerTicks(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0B |= (1 << CS00) | (1 << CS02);                 /* 8 MHz / 1024 */
  OCR0A = 7;                            /* 8 Mhz / 1024 / 8 = 1.024 ms */
  TIMSK0 |= (1 << OCIE0A);          /* output compare interrupt enable */
  sei();                          /* set (global) enable interrupt bit */
}

ISR(TIMER0_COMPA_vect) {
  milliseconds++;
}

int main(void) {
  uint8_t ledTime[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t i;

  // -------- Inits --------- //
  initTimerTicks();
  LED_DDR = 0xff;                                        /* all output */

  // ------ Event loop ------ //
  while (1) {
                             /* 8 LEDs flashing on different schedules */
    for (i = 0; i < 8; i++) {
      if (milliseconds == ledTime[i]) {
        LED_PORT ^= (1 << i);
        ledTime[i] = milliseconds + 100 + i;
      }
    }
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
