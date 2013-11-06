

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "pinDefines.h"

#include "USART.h"

#define DEBOUNCE_TIME  10                              /* milliseconds */

// -------- Global Variables --------- //
volatile uint8_t milliseconds = 0;
volatile uint8_t debouncing;
volatile uint8_t debounceTime;


// -------- Functions --------- //
ISR(TIMER0_COMPA_vect) {
  milliseconds++;
}

ISR(INT0_vect) {                            /* Run when button pressed */
  debounceTime = milliseconds + DEBOUNCE_TIME;
  debouncing = 1;
}

static inline void initTimerTicks(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0B |= (1 << CS02);                                /* 8 MHz / 256 */
  TIMSK0 |= (1 << OCIE0A);          /* output compare interrupt enable */
  OCR0A = 31;                           /* 8 Mhz / 256 / 31 = 0.992 ms */
  sei();                          /* set (global) enable interrupt bit */
}

static inline void initInterrupt0(void) {
  EIMSK |= (1 << INT0);                                 /* enable INT0 */
  EICRA |= (1 << ISC01);                    /* trigger on falling edge */
  sei();                          /* set (global) interrupt enable bit */
}


int main(void) {
  uint8_t ledTime[6];
  uint8_t i;

  // -------- Inits --------- //
  initTimerTicks();
  initInterrupt0();
  LED_DDR = 0xff;                                        /* all output */
  BUTTON_PORT |= (1 << BUTTON);                              /* pullup */

  // ------ Event loop ------ //
  while (1) {

                              /* LEDs flashing on un-related schedules */
                                          /* Gives CPU something to do */
                        /* Also a stand-in for tricky scheduling tasks */
    for (i = 0; i < 6; i++) {
      if (milliseconds == ledTime[i]) {
        LED_PORT ^= (1 << i);
        ledTime[i] = milliseconds + 100 + i;
                                          /* next toggle in 100 + i ms */
      }
    }

                   /* If debouncing and time is up, test again and act */
    if (debouncing && (milliseconds == debounceTime)) {
      if (bit_is_clear(BUTTON_PIN, BUTTON)) {
        LED_PORT ^= (1 << LED7);
      }
      debouncing = 0;
    }

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
