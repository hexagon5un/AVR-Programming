#include "timerTicks.h"

void initTicks() {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0B |= (1 << CS00) | (1 << CS02);                 /* 8 MHz / 1024 */
  TIMSK0 |= (1 << OCIE0A);          /* output compare interrupt enable */
  OCR0A = 77;                        /* 78 / (8MHz / 1024) =  9.984 ms */
  sei();                          /* set (global) enable interrupt bit */
  ticks = 0;
}

ISR(TIMER0_COMPA_vect) {
  ticks++;
}

void sleepDelay(uint16_t numTicks) {
  numTicks += ticks;                    /* when should I wake back up? */
  while (!(ticks == numTicks)) {
    sleep_mode();
  }
}


