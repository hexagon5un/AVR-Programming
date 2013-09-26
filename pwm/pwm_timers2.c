/*

PWM Demo with serial control over three LEDs

*/

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"

#define LED_DELAY  5                                   /* milliseconds */

static inline void initTimers(void) {
  // Timer 1 A,B
  set_bit(TCCR1A, WGM10);                      /* Fast PWM mode, 8-bit */
  set_bit(TCCR1B, WGM12);                       /* Fast PWM mode, pt.2 */
  set_bit(TCCR1B, CS11);            /* F_CPU/8 -- 1MHz with fast fuses */

  set_bit(TCCR1A, COM1A1);                      /* PWM output on OCR1A */
  set_bit(TCCR1A, COM1B1);                      /* PWM output on OCR1B */
  set_bit(LED_DDR, LED1);                      /* enable output on pin */
  set_bit(LED_DDR, LED2);                      /* enable output on pin */

  // Timer 2
  set_bit(TCCR2A, WGM20);                             /* Fast PWM mode */
  set_bit(TCCR2A, WGM21);                       /* Fast PWM mode, pt.2 */
  set_bit(TCCR2B, CS21);            /* F_CPU/8 -- 1MHz with fast fuses */

  set_bit(TCCR2A, COM2A1);                      /* PWM output on OCR2A */
  set_bit(LED_DDR, LED3);                      /* enable output on pin */
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
