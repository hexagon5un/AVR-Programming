/* Blinker Demo */

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */


int main(void){

  // -------- Inits --------- //
  DDRB = (1 << PB0);		/* Data Direction Register B:
				   writing a one to the bit 
				   enables output. */

  // ------ Event loop ------ //
  while(1){			

    PORTB = (1 << PB0);		/* Turn on the LED bit/pin in PORTB */
    _delay_ms(200);		/* wait */

    PORTB = 0;			/* Turn off all B pins, including LED */
    _delay_ms(200);		/* wait */

  } /* End event loop */
  return(0);			/* This line is never reached  */
}




