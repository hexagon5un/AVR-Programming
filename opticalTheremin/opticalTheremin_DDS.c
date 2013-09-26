#include "fullTri7.h"

// -------- Global Variables --------- //

volatile uint16_t accumulator;
volatile uint16_t tuningWord;
volatile uint8_t volume;

// -------- Functions --------- //

static inline void initTimer0(void) {
                                     /* Fast PWM mode, output on OCR0A */
  TCCR0A |= ((1 << WGM00) | (1 << WGM01) | (1 << COM0A1));
  TCCR0B |= (1 << CS00);                    /* Clock with /1 prescaler */
  TIMSK0 |= (1 << TOIE0);                        /* Overflow interrupt */
}

ISR(TIMER0_OVF_vect) {
  int8_t mixer;
  mixer = fullTri7[(uint8_t) (accumulator >> 8)];        /* lookup DDS */
  mixer = ((mixer * volume) >> 8);   /* multiply by volume and rescale */
  OCR0A = 128 + mixer;                              /* recenter output */
  accumulator += tuningWord;          /* take tuningWord steps forward */
}
