/* 
  POV demo with words
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  1000000UL
#include <util/delay.h>
#define DELAYTIME 1		/* ms */

// #include "uppercase.h"
#include "thinFont.h"  	   /* provides font[][5] array */


void POVString(char *myString){
  uint8_t letterNum;
  uint8_t column;
  letterNum = 0;
  while(myString[letterNum] != 0){

    /* display character */
    for (column = 0; column < 5; column++){ 
      PORTB = font[myString[letterNum] - ' '][column];
      _delay_ms(DELAYTIME);
    }  
 
    // Delay between characters, go to next letter
    PORTB = 0;
    _delay_ms(DELAYTIME);
    _delay_ms(DELAYTIME);
    _delay_ms(DELAYTIME);
    letterNum++;
  }
}



int main(void){
  DDRB = 0xff;              /* all output */
  uint8_t i;

  while(1){			/* mainloop */

      
    POVString("AAAA");
    for (i=0; i<10; i++){	/* long delay after whole word */
      _delay_ms(DELAYTIME);
    }

    POVString("aaaa"); 
    for (i=0; i<10; i++){	/* long delay after whole word */
      _delay_ms(DELAYTIME);
    }


  } /* end mainloop */
  return(0);
}
