/*
   Demo of the way _not_ to do simple button presses
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  1000000UL
#include <util/delay.h>

#define BUTTON         PD6
#define BUTTON_PORT    PORTD
#define BUTTON_PINS    PIND

#define OUTPUT_PORT   PORTB
#define OUTPUT_DDR    DDRB

#define LOOP_DELAY    100

uint8_t incrementLED(uint8_t ledBits) {
  ledBits = ledBits << 1;                          /* roll to the left */

  if (!ledBits) {                           /* check if no bits remain */
    ledBits = 0b00000001;                      /* enable the first bit */
  }

  return (ledBits);
}

void blinkAll(void) {
  // blink all as a sanity check
  OUTPUT_PORT = 0xff;
  _delay_ms(100);
  OUTPUT_PORT = 0x00;
  _delay_ms(200);
  OUTPUT_PORT = 0xff;
  _delay_ms(100);
  OUTPUT_PORT = 0x00;
  _delay_ms(200);
}


int main(void) {
  uint8_t ledState;

                        /* initialize pullup resistor on our input pin */
  BUTTON_PORT = (1 << BUTTON);
  OUTPUT_DDR = 0xff;                         /* set up LEDs for output */

  blinkAll();

  while (1) {                                              /* mainloop */

                              /* light up next pin when button pressed */
    if (bit_is_clear(BUTTON_PINS, BUTTON)) {  /* pin is negative logic */
      OUTPUT_PORT = incrementLED(OUTPUT_PORT);
    }


    _delay_ms(LOOP_DELAY);

  }
  return (0);
}
