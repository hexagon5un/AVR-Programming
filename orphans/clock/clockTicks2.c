                       /* Example of setting up a near-real-time timer */

// ------- Preamble -------- //
#include <avr/io.h>
#include <avr/interrupt.h>

#include "pinDefines.h"


// -------- Global Variables --------- //
volatile uint8_t ticks = 0;           /* 196 cycles ~= 25 milliseconds */

// -------- Functions --------- //
ISR(TIMER0_COMPA_vect) {
  ticks++;
}

static inline void initTimerTicks(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0B |= (1 << CS00);
  TCCR0B |= (1 << CS02);                    /* 8 MHz / 1024: 7812.5 Hz */
  TIMSK0 |= (1 << OCIE0A);          /* output compare interrupt enable */
  OCR0A = 195;                          /* 8 Mhz / 1024 / 196 ~= 25 ms */
  sei();                                   /* set enable interrupt bit */
}

int main(void) {
  // -------- Inits --------- //
  initTimerTicks();
  BUTTON_PORT |= (1 << BUTTON);             /* enable pullup on button */
  uint8_t nextTick;
  enum { released, pressed, debouncing } buttonState;
  buttonState = released;        /* set initially to non-pressed state */

  // ------ Event loop ------ //
  while (1) {

    if (bit_is_set(BUTTON_PIN, BUTTON)) {        /* seems to be pressed */
      if (buttonState != debouncing) { /* not yet debouncing, so start */
        buttonState = debouncing;
        nextTick = ticks + 1;
      }
      else                                /* is debouncing, check time */
        buttonState = pressed;
    }
  }
  else {                                             /* if not pressed */
    buttonState = released;
  }


}                                                    /* End event loop */

return (0);                              /* This line is never reached */
}
