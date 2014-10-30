/*

Demo of using interrupts for doing what they do best --
two things at once.

Flashes LED0 at a fixed rate, interrupting whenever button is pressed.

 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"

ISR(INT0_vect) {         /* Run every time there is a change on button */
  if (bit_is_clear(BUTTON_PIN, BUTTON)) {
    LED_PORT |= (1 << LED1);
  }
  else {
    LED_PORT &= ~(1 << LED1);
  }
}

void initInterrupt0(void) {
  EIMSK |= (1 << INT0);                                 /* enable INT0 */
  EICRA |= (1 << ISC00);                /* trigger when button changes */
  sei();                          /* set (global) interrupt enable bit */
}

int main(void) {
  // -------- Inits --------- //
  LED_DDR = 0xff;                                   /* all LEDs active */
  BUTTON_PORT |= (1 << BUTTON);                              /* pullup */
  initInterrupt0();

  // ------ Event loop ------ //
  while (1) {

    _delay_ms(200);
    LED_PORT ^= (1 << LED0);

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
