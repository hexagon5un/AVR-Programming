/* Blinker Demo */

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "../macros.h"	


int main(void){

  // -------- Inits --------- //
  set_bit(DDRB, PB0);		/* Data Direction Register B:
				   writing a one to the bit 
				   enables output. */

  // ------ Event loop ------ //
  while(1){			

    PORTB = BV(PB0);	      /* Turn on first LED bit/pin in PORTB */
    _delay_ms(1000);	      /* wait */

    PORTB = 0b00000000;	      /* Turn off all B pins, including LED */
    _delay_ms(1000);	      /* wait */

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



