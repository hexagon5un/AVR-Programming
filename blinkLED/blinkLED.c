/* Blinker Demo */

// Preamble
#include <avr/io.h>		/* Defines pins, ports, etc */
#define F_CPU 1000000UL	        /* Sets up the chip speed for delay.h */
#include <util/delay.h>		/* Functions to waste time */

#define LED  PB0		/* Defines pin PB0 for the LED.  I
				 often include a bunch of the circuit
				 info in the code this way, which
				 makes porting the code to another
				 chip easier and reminds you of how to
				 hook it up. */

// Here is the main body of the program
int main(void){

  // Initialization code goes here
  DDRB = (1 << LED);		/* Data Direction Register B:
				   writing a one to the bit 
				   enables output. */

  // And then here is the main loop, from which we never return
  while(1){			

    PORTB = (1 << LED); 		/* Turn on the LED bit/pin in PORTB */
    _delay_ms(200);		/* wait */

    PORTB &= ~(1 << LED);		/* Turn off LED */
   _delay_ms(200);		/* wait */

  }
  return(0);			/* This line is never reached  */
}




