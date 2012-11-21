/* Blinker Demo */

// Preamble
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */


// Here is the main body of the program
int main(void){

  // Initialization code goes here
  DDRB = (1 << PB0);		/* Data Direction Register B:
				   writing a one to the bit 
				   enables output. */

  // And then here is the main loop, from which we never return
  while(1){			

    PORTB = (1 << PB0);		/* Turn on the LED bit/pin in PORTB */
    _delay_ms(200);		/* wait */

    PORTB = 0;			/* Turn off all B pins, including LED */
    _delay_ms(200);		/* wait */

  }
  return(0);			/* This line is never reached  */
}




