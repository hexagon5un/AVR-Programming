                       /* Example of setting up a near-real-time timer */

// ------- Preamble -------- //
#include <avr/io.h>
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"

// -------- Global Variables --------- //
volatile uint8_t ticks = 0;           /* 196 cycles ~= 25 milliseconds */
volatile uint8_t seconds = 0;                   /* 40 ticks = 1 second */
volatile uint8_t minutes = 0;                     /* 60 sec = 1 minute */
volatile uint8_t ledState = 0;                   /* start all LEDs off */

// -------- Functions --------- //
ISR(TIMER0_COMPA_vect) {
  ticks++;                                  /* roughly 25 milliseconds */
  ledState ^= (1 << LED0);                          /* toggle LED0 bit */

  if (ticks == 40) {                           /* roughly every second */
    ticks = 0;
    seconds++;
    ledState ^= (1 << LED1);                        /* toggle LED1 bit */
  }

  if (seconds == 60) {                         /* roughly every minute */
    seconds = 0;
    minutes++;
    ledState ^= (1 << LED2);                        /* toggle LED2 bit */
  }
}

static inline void initTimerTicks(void) {
  set_bit(TCCR0A, WGM01);                                  /* CTC mode */
  set_bit(TCCR0B, CS00);
  set_bit(TCCR0B, CS02);                    /* 8 MHz / 1024: 7812.5 Hz */
  set_bit(TIMSK0, OCIE0A);          /* output compare interrupt enable */
  OCR0A = 195;                          /* 8 Mhz / 1024 / 196 ~= 25 ms */
  sei();                                   /* set enable interrupt bit */
}

int main(void) {

  // -------- Inits --------- //
  initTimerTicks();
  LED_DDR = (1 << LED0) | (1 << LED1) | (1 << LED2);

  // ------ Event loop ------ //
  while (1) {

    LED_PORT = ledState;

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
