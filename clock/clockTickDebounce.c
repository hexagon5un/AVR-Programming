                       /* Example of setting up a near-real-time timer */

// ------- Preamble -------- //
#include <avr/io.h>
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"

// -------- Global Variables --------- //
volatile uint8_t ticks = 0;           /* 196 cycles ~= 25 milliseconds */

// -------- Functions --------- //
ISR(TIMER0_COMPA_vect) {
  ticks++;
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
  BUTTON_PORT |= (1 << BUTTON);             /* enable pullup on button */
  LED_DDR = 0xff;                               /* all LEDs for output */
  LED_PORT = (1 << 0);                      /* start with first LED on */

  uint8_t nextTick;
  enum { released, pressed, debouncing } buttonState = released;
                                 /* set initially to non-pressed state */

  // ------ Event loop ------ //
  while (1) {

    if (bit_is_set(BUTTON_IN, BUTTON)) {        /* seems to be pressed */
      if (buttonState != debouncing) { /* not yet debouncing, so start */
        buttonState = debouncing;
        nextTick = ticks + 1;
      }
      else                                /* is debouncing, check time */
        buttonState = pressed;
      LED_PORT = LED_PORT << 1;                 /* advance to next LED */
      if (!LED_PORT) {                /* wrap back around to first LED */
        LED_PORT = (1 << 0);
      }
    }
  }
  else {                                      /* release not debounced */
    buttonState = released;
  }


}                                                    /* End event loop */

return (0);                              /* This line is never reached */
}
