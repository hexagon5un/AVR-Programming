     /* Demo using pin-change interrupts and in-ISR debouncing routine */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"


ISR(PCINT2_vect) {              /* Run every time button state changes */
  if (bit_is_clear(BUTTON_PIN, BUTTON)) {
    LED_PORT |= (1 << LED1);
  }
  else {
    LED_PORT &= ~(1 << LED1);
  }
}

void initPinChangeInterrupt18(void) {
  PCICR |= (1 << PCIE2);        /* set pin-change interrupt for D pins */
  PCMSK2 |= (1 << PCINT18);      /* set mask to look for PCINT18 / PD2 */
  sei();                          /* set (global) interrupt enable bit */
}

int main(void) {
  // -------- Inits --------- //
  LED_DDR = 0xff;                                   /* all LEDs active */
  BUTTON_PORT |= (1 << BUTTON);                              /* pullup */
  initPinChangeInterrupt18();

  // ------ Event loop ------ //
  while (1) {

    _delay_ms(200);
    LED_PORT ^= (1 << LED0);

  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}
