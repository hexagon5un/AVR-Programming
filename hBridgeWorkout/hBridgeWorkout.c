// Simple demo of an h-bridge 

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"

static inline void setBridgeState(uint8_t bridgeA, uint8_t bridgeB) {
  /* Utility function that lights LEDs when it energizes a bridge side */
  if (bridgeA) {
    PORTD |= (1 << PD6);
    LED_PORT |= (1 << LED0);
  }
  else {
    PORTD &= ~(1 << PD6);
    LED_PORT &= ~(1 << LED0);
  }
  if (bridgeB) {
    PORTD |= (1 << PD5);
    LED_PORT |= (1 << LED1);
  }
  else {
    PORTD &= ~(1 << PD5);
    LED_PORT &= ~(1 << LED1);
  }
}


int main(void) {
  // -------- Inits --------- //

  DDRD |= (1 << PD6);               /* now hooked up to bridge, input1 */
  DDRD |= (1 << PD5);               /* now hooked up to bridge, input2 */
  LED_DDR |= (1 << LED0);
  LED_DDR |= (1 << LED1);

  // ------ Event loop ------ //
  while (1) {

    setBridgeState(1, 0);                                 /* "forward" */
    _delay_ms(2000);

    setBridgeState(0, 0);                      /* both low stops motor */
    _delay_ms(2000);

    setBridgeState(0, 1);                                 /* "reverse" */
    _delay_ms(2000);

    setBridgeState(1, 1);                /* both high also stops motor */
    _delay_ms(2000);

    // For extra-quick braking, energize the motor backwards
    setBridgeState(1, 0);
    _delay_ms(2000);
    setBridgeState(0, 1);
    _delay_ms(75);              /* tune this time to match your system */
    setBridgeState(0, 0);
    _delay_ms(2000);

  }                                                  /* End event loop */
  return (0);
}
