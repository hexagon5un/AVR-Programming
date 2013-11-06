                                           /* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include <avr/interrupt.h>
#include "pinDefines.h"

#include "fullSine.h"


static inline void initTimer0(void) {
  TCCR0A |= (1 << COM0A1);                      /* PWM output on OCR0A */
  SPEAKER_DDR |= (1 << SPEAKER);               /* enable output on pin */

  TCCR0A |= (1 << WGM00);                             /* Fast PWM mode */
  TCCR0A |= (1 << WGM01);                       /* Fast PWM mode, pt.2 */

  TCCR0B |= (1 << CS00);                    /* Clock with /1 prescaler */
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

static inline void initLED(void) {
  LED_DDR |= (1 << LED0);                    /* LED on for diagnostics */
  LED_PORT |= (1 << LED0);
  _delay_ms(100);
  LED_PORT &= ~(1 << LED0);
}

int main(void) {

  volatile uint16_t accumulator0;
  volatile uint16_t accumulator1;
  volatile uint16_t tuningWord0;
  volatile uint16_t tuningWord1;
  volatile uint16_t mixer;

  // -------- Inits --------- //

  initLED();
  initTimer0();

  BUTTON_PORT |= (1 << BUTTON);                    /* pullup on button */
  SPEAKER_DDR |= (1 << SPEAKER);                     /* speaker output */

  tuningWord0 = 440 * 2;                      /* Dial tone frequencies */
  tuningWord1 = 350 * 2;

  // ------ Event loop ------ //
  while (1) {

    loop_until_bit_is_set(TIFR0, TOV0); /* wait until overflow bit set */
    TIFR0 |= (1 << TOV0);               /* writing set should reset... */

    //  Note that this bit has to be pretty fast... we have only 256 cycles to
