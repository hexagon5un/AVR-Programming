/* Showing off some patterns to practice our bit-twiddling */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>  

#define DELAYTIME 85		/* milliseconds */
#define LED_PORT                PORTB
#define LED_DDR                 DDRB

int main(void){
  uint8_t i;
  uint8_t repetitions;
  uint16_t randomNumber = 1234;
  uint8_t whichLED; 
  // -------- Inits --------- //
  LED_DDR = 0xff;    		/* all LEDs configured for output */
  // ------ Event loop ------ //
  while(1){     
    /* Go Left */
    for (i=0; i<8; i++){
      LED_PORT |= (1 << i);	/* turn on the i'th pin */
      _delay_ms(DELAYTIME);		/* wait */
    }
    for (i=0; i<8; i++){
      LED_PORT &= ~(1 << i);	/* turn on the i'th pin */
      _delay_ms(DELAYTIME);		/* wait */
    }
    _delay_ms(5*DELAYTIME);	/* pause */
   
    /* Go Right */
    for (i=7; i<255; i--){
      LED_PORT |= (1 << i);	/* turn on the i'th pin */
      _delay_ms(DELAYTIME);		/* wait */     
    }
    for (i=7; i<255; i--){
      LED_PORT &= ~(1 << i);	/* turn on the i'th pin */
      _delay_ms(DELAYTIME);		/* wait */
    }
    _delay_ms(5*DELAYTIME);   	/* pause */

    /* Toggle at random for a while */
    for (repetitions=0; repetitions < 75; repetitions++){
      /* super-easy pseudo-random number generation */
      randomNumber = 2053*randomNumber + 13849; 
      whichLED = (randomNumber >> 8);       /* keep top 8 bits */
      whichLED = whichLED & 0b00000111;	    /* reduce to range 0-7 */
      LED_PORT ^= (1 << whichLED);	    /* toggle our LED */
      _delay_ms(DELAYTIME);
    }
    LED_PORT = 0;		/* all LEDs off */
    _delay_ms(5*DELAYTIME);   	/* pause */

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

