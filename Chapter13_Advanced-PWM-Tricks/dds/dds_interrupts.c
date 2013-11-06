                                           /* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include <avr/interrupt.h>
#include "pinDefines.h"

#include "fullSine.h"
#include "USART.h"

volatile uint16_t accumulator;
volatile uint16_t tuningWord;

static inline void initTimer0(void) {
  TCCR0A |= (1 << COM0A1);                      /* PWM output on OCR0A */
  SPEAKER_DDR |= (1 << SPEAKER);               /* enable output on pin */

  TCCR0A |= (1 << WGM00);                             /* Fast PWM mode */
  TCCR0A |= (1 << WGM01);                       /* Fast PWM mode, pt.2 */

  TCCR0B |= (1 << CS00);                    /* Clock with /1 prescaler */
  TIMSK0 |= (1 << TOIE0);                        /* Overflow interrupt */
  sei();                                           /* Enable interrupt */
}

ISR(TIMER0_OVF_vect) {

  accumulator += tuningWord;          /* take tuningWord steps forward */
                                              /* lookup and set output */
  OCR0A = 128 + fullSine[(uint8_t) (accumulator >> 8)];

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

  // -------- Inits --------- //

  initTimer0();
  initUSART();

  BUTTON_PORT |= (1 << BUTTON);                    /* pullup on button */

  LED_DDR |= (1 << LED0);                    /* LED on for diagnostics */
  LED_PORT |= (1 << LED0);
  _delay_ms(100);
  LED_PORT &= ~(1 << LED0);

  SPEAKER_DDR |= (1 << SPEAKER);

  tuningWord = 880;
  transmitByte((uint8_t) tuningWord >> 2);       /* approx value in Hz */

  // ------ Event loop ------ //
  while (1) {

    pollButton();

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
