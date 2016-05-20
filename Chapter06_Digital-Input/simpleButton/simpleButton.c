/*
   Demo of the simplest on/off button code
   Button connected to PD2
   LEDs connected to PB0..PB7
*/

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // -------- Inits --------- //
  PORTD |= (1 << PD2);  /* initialize pullup resistor on our input pin */
  DDRB = 0xff;                           /* set up all LEDs for output */

  // ------ Event loop ------ //
  while (1) {
    if (bit_is_clear(PIND, PD2)) {            /* look for button press */
                      /* equivalent to if ((PIND & (1 << PD2)) == 0 ){ */
      PORTB = 0b00111100;                                   /* pressed */
    }
    else {                                              /* not pressed */
      PORTB = 0b11000011;
    }
  }                                                  /* End event loop */
  return 0;
}
