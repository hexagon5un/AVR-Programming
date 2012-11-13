#include <avr/io.h>		/* Defines pins, ports, etc */
#define F_CPU 1000000UL	        /* Sets up the chip speed for delay.h */
#include <util/delay.h>		/* Functions to waste time */

#define DELAYTIME 75         /* milliseconds, 50-75 is good for smooth eyes */

void main(void){
  
  uint8_t i;

  DDRB = 0b11111111;		      /* Data Direction Register B:
					 all set up for output */
  
  while(1){			/* mainloop */
    
    for (i=0; i<=6; i++){	/* count i from 0 to 6 */	
      PORTB = (1 << i);		/* illuminate only pin i */
      _delay_ms(DELAYTIME);	/* wait */

      /* This part is optional, but I find it adds some extra
	 cyloniness (cylonity?) to the whole thing */
      PORTB = (1 << i) | (1 << i+1); 
      _delay_ms(DELAYTIME/2); 
    }

    for(i=7; i>=1; i--){	/* count i from 7 to 1 */
      PORTB = (1 << i);
      _delay_ms(DELAYTIME);          

      /* Optional counterpart to above */
      PORTB = (1 << i) | (1 << i-1);
      _delay_ms(DELAYTIME/2);
    }

  } /* end mainloop */
} /* end main */

