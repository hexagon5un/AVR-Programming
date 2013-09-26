/*
   Demo of the way _not_ to do simple button presses
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  8000000UL
#include <util/delay.h>

#define INPUT_PIN     PD2
#define INPUT_PORT    PORTD
#define INPUT_DDR     DDRD
#define INPUT_INPUT   PIND

#define OUTPUT_PORT PORTB
#define OUTPUT_DDR  DDRB

                                                    /* Global variable */
uint8_t led = 0;

static inline void incrementLED(void) {
  if (led == 7) {
    led = 0;                                            /* wrap around */
  }
  else if (led < 7) {
    led++;                                              /* or count up */
  }
}

void main(void) {

                        /* initialize pullup resistor on our input pin */
  INPUT_PORT = (1 << INPUT_PIN);
  OUTPUT_DDR = 0xff;                         /* set up LEDs for output */

                                        /* blink all as a sanity check */
  OUTPUT_PORT = 0xff;
  _delay_ms(100);
  OUTPUT_PORT = 0x00;
  _delay_ms(200);


  while (1) {                                              /* mainloop */

                              /* light up next pin when button pressed */
    if (!(INPUT_INPUT & (1 << INPUT_PIN))) {  /* pin is negative logic */
      incrementLED();
      _delay_ms(400);            /* add in pause to allow single press */
    }

    OUTPUT_PORT = (1 << led);

  }                                                    /* end mainloop */

}
