/*

PWM Demo with serial control over three LEDs

*/

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"


#define LED_DELAY  5                                   /* milliseconds */

static inline void initTimers(void) {
  // Timer 1 A,B
  TCCR1A |= (1 << WGM10);                      /* Fast PWM mode, 8-bit */
  TCCR1B |= (1 << WGM12);                       /* Fast PWM mode, pt.2 */
  TCCR1B |= (1 << CS11);            /* F_CPU/8 -- 1MHz with fast fuses */

  TCCR1A |= (1 << COM1A1);                      /* PWM output on OCR1A */
  TCCR1A |= (1 << COM1B1);                      /* PWM output on OCR1B */
  LED_DDR |= (1 << LED1);                      /* enable output on pin */
  LED_DDR |= (1 << LED2);                      /* enable output on pin */

  // Timer 2
  TCCR2A |= (1 << WGM20);                             /* Fast PWM mode */
  TCCR2A |= (1 << WGM21);                       /* Fast PWM mode, pt.2 */
  TCCR2B |= (1 << CS21);            /* F_CPU/8 -- 1MHz with fast fuses */

  TCCR2A |= (1 << COM2A1);                      /* PWM output on OCR2A */
  LED_DDR |= (1 << LED3);                      /* enable output on pin */
}

int main(void) {

  // -------- Inits --------- //

  initTimers();

  // ------ Event loop ------ //
  while (1) {

    // Brighten
    while (OCR2A < 255) {
      OCR2A++;
      OCR1A++;
      OCR1B++;
      _delay_ms(LED_DELAY);
    }

    // Dim
    while (OCR2A > 0) {
      OCR2A--;
      OCR1A--;
      OCR1B--;
      _delay_ms(LED_DELAY);
    }

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
