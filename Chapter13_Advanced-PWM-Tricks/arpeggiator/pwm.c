

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"


static inline void initTimer0(void) {

  TCCR0A |= (1 << COM0A1);                      /* PWM output on OCR0A */
  SPEAKER_DDR |= (1 << SPEAKER);               /* enable output on pin */

  TCCR0A |= (1 << WGM00);                             /* Fast PWM mode */
  TCCR0A |= (1 << WGM01);                       /* Fast PWM mode, pt.2 */

  TCCR0B |= (1 << CS00);                    /* Clock with /1 prescaler */

  //  TIMSK0 |= (1 << TOIE0);                    /* Overflow interrupt */
  //  sei();                                       /* Enable interrupt */
}

static inline void pollButton(void) {
  if (bit_is_clear(BUTTON_PIN, BUTTON)) {
    SPEAKER_DDR |= (1 << SPEAKER);             /* enable output on pin */
    LED_PORT &= ~(1 << LED0);
  }
  else {
    SPEAKER_DDR &= ~(1 << SPEAKER);          /* disable output on pin */
    LED_PORT |= (1 << LED0);
  }
}

int main(void) {
  uint8_t i;
  uint8_t volume;
  uint8_t increasingVolume;


  // -------- Inits --------- //
  initTimer0();

  BUTTON_PORT |= (1 << BUTTON);                    /* pullup on button */

  LED_DDR |= (1 << LED0);                    /* LED on for diagnostics */
  LED_PORT |= (1 << LED0);
  _delay_ms(100);
  LED_PORT &= ~(1 << LED0);

  SPEAKER_DDR |= (1 << SPEAKER);
  OCR0A = 210;
  // ------ Event loop ------ //
  while (1) {

    if (increasingVolume) {
      volume++;
    }
    else {
      volume--;
    }

    if (volume > 120) {
      increasingVolume = 0;
    }
    if (volume < 2) {
      increasingVolume = 1;
    }

    // Increase
    while (i < (250 - volume)) {
      i++;
      //_delay_us(15);
      OCR0A = i;
      pollButton();
    }
    // Decrease
    while (i > 5 + volume) {
      i--;
      //_delay_us(15);
      OCR0A = i;
      pollButton();
    }

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
