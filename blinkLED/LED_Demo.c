/* Blinker Demo */

#include <avr/io.h>		/* Defines pins, ports, etc */
#define F_CPU 8000000UL	        /* Sets up the chip speed for delay.h */
#include <util/delay.h>		/* Functions to waste time */

#define LED  PB0		/* Defines pin PB0 for the LED.  I
				 often include a bunch of the circuit
				 info in the code this way, which
				 makes porting the code to another
				 chip easier and reminds you of how to
				 hook it up. */

void main(void){

  DDRB = _BV(LED);		      /* Data Direction Register B:
				       writing a one to the bit
				       enables output.  More on the
				       _BV() macro in the next
				       lesson. */
	
  while(1){			/* the main loop, from which we never return */

    PORTB = _BV(LED); 		/* Turn on the LED bit/pin in PORTB */
    _delay_ms(400);		/* wait */

    PORTB = 0;		       /* Turn off everything(!) on PORTB */
   _delay_ms(400);		/* wait */

  }
}




