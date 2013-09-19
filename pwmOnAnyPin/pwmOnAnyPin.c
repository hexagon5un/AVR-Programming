// Quick and dirty demo of how to get PWM on any pin with interrupts
// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"

#define DELAY 3

volatile uint8_t brightnessA;
volatile uint8_t brightnessB;

// -------- Functions --------- //
static inline void initTimer0(void) {
                                 /* must be /64 or more for ISR timing */
  TCCR0B |= (1 << CS01) | (1 << CS00);
                                     /* both output compare interrupts */
  TIMSK0 |= ((1 << OCIE0A) | (1 << OCIE1B));
  TIMSK0 |= (1 << TOIE0);                 /* overflow interrupt enable */
  sei();
}

ISR(TIMER0_OVF_vect) {
  LED_PORT = 0xff;
  OCR0A = brightnessA;
  OCR0B = brightnessB;
}
ISR(TIMER0_COMPA_vect) {
  LED_PORT &= 0b11110000;                    /* turn off low four LEDs */
}
ISR(TIMER0_COMPB_vect) {
  LED_PORT &= 0b00001111;                   /* turn off high four LEDs */
}
#define SCALE 5 
int main(void) {
  // -------- Inits --------- //

  uint8_t i;
  LED_DDR = 0xff;
  initTimer0();

  // ------ Event loop ------ //
  while (1) {

    while (i < 250) {
      _delay_ms(DELAY);
      brightnessA = i;
      brightnessB = 255 - i;
			i += SCALE;
		}

    while(i > 10) {
      _delay_ms(DELAY);
      brightnessA = i;
      brightnessB = 255 - i;
			i -= SCALE;
		}

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
