#include <avr/io.h>		/* Defines pins, ports, etc */
#define F_CPU 8000000UL	        /* Sets up the chip speed for delay.h */
#include <util/delay.h>		/* Functions to waste time */

#define DELAYTIME 75		/* milliseconds */

void main(void){

  uint8_t i;

  DDRB = 0b11111111;		      /* Data Direction Register B:
					 all set up for output */
  
  while(1){			/* mainloop */

    for (i=0; i<=6; i++){	/* count i up from 0 to 6 */
      PORTB = _BV(i);		/* illuminate only i'th pin */
      _delay_ms(DELAYTIME);	/* wait */
    }

    for (i = 7; i >= 1; i--){	/* count i down from 7 to 1 */
      PORTB = _BV(i);		
      _delay_ms(DELAYTIME);    
    }
  } /* end mainloop */
}   /* end main */

