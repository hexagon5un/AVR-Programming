                                           /* Quick and dirty PWM Demo */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"

#define LED_DELAY  20                                  /* microseconds */

void pwmAllPins(uint8_t brightness) {
  uint8_t i;
  LED_PORT = 0xff;                                          /* turn on */
  for (i = 0; i < 255; i++) {
    if (i >= brightness) {            /* once it's been on long enough */
      LED_PORT = 0;                                        /* turn off */
    }
    _delay_us(LED_DELAY);
  }
}

int main(void) {

  uint8_t brightness = 0;
  int8_t direction = 1;

  // -------- Inits --------- //

  // Init all LEDs
  LED_DDR = 0xff;
  // ------ Event loop ------ //
  while (1) {
    // Brighten and dim
    if (brightness == 0) {
      direction = 1;
    }
    if (brightness == 255) {
      direction = -1;
    }
    brightness += direction;
    pwmAllPins(brightness);


  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
