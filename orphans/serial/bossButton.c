/*
bossButton.c

As long as we've got a button wired up to the AVR,
might as well have some fun.

Upload this code to your AVR, run bossButton.py.
Press button.

*/

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"

static inline void blinkLED(void) {
  LED_PORT = (1 << LED);
  _delay_ms(200);
  _delay_ms(200);
  _delay_ms(200);
  _delay_ms(200);
  LED_PORT &= ~(1 << LED);
}


int main(void) {
  char serialCharacter;

  // -------- Inits --------- //
  BUTTON_PORT |= (1 << BUTTON);          /* input mode, turn on pullup */

  LED_DDR = (1 << LED);
  blinkLED();

  initUSART();
  transmitByte('O');

  // ------ Event loop ------ //
  while (1) {

    if (bit_is_clear(BUTTON_PIN, BUTTON)) {
      transmitByte('X');
      blinkLED();
    }

  }                                                  /* End event loop */
  return (0);
}
