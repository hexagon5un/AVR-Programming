
/* 
   Direct-digital synthesis
   Mixer Demo
   
*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include <avr/interrupt.h>	
#include "pinDefines.h"
#include "macros.h"
#include "fullTriangle.h"
#include "USART.h"

#define BASEPITCH       800 /* in tuningWord steps, roughly 1/2 Hz */
#define INITIAL_DETUNE  64  /* how many updates until all together */
#define SPEED_FACTOR    8   /* 1,2,4,8 are good. Odd values not good. */

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
}


static inline void initLEDs(void){
  uint8_t i;
  LED_DDR = 0xff;	/* All LEDs for diagnostics */
  for (i=0; i<8; i++){
    set_bit(LED_PORT, i);
    _delay_ms(100);
    clear_bit(LED_PORT, i);
  }
}

int main(void){

  uint16_t accumulators[8];  
  uint16_t tuningWords[8];
  uint16_t mixer;
  uint8_t  i;
  uint16_t cycles;
  uint8_t  counter=0;

  // -------- Inits --------- //
  
  initLEDs();
  initTimer0();
  initUSART();
  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  // Start off each on a different pitch, but in phase
  for (i=0; i<8; i++){
    tuningWords[i] = BASEPITCH - INITIAL_DETUNE*i;
    accumulators[i] = 0;
  }
  

  // ------ Event loop ------ //
  while(1){		       
    
    loop_until_bit_is_set(TIFR0, TOV0); /* wait until overflow bit set */
    set_bit(TIFR0, TOV0);		/* writing set should reset... */
    OCR0A = 128 + mixer;	

    /* Update all accumulators, mix together */
    mixer = 0;
    for (i=0; i<8; i++){
      accumulators[i] += tuningWords[i];
      mixer += fullTriangle[(uint8_t) (accumulators[i] >> 8)]; // triangle version
    }
    mixer = mixer >> 3;		/* quick divide by 8 */

    cycles += SPEED_FACTOR;	/* count how many cycles we've been through */
    
    // Each one drifts every more slightly into pitch
    if (cycles == 0){
      LED_PORT = INITIAL_DETUNE - counter;	/* display countdown */
      if (counter < INITIAL_DETUNE){		/* move a step toward in-tune */
	for (i=0; i<8; i++){
	  tuningWords[i] += i;
	}
      }
      counter++;		/* count number steps */
      if (counter > INITIAL_DETUNE + 3){ /* and then turn off */
	clear_bit(SPEAKER_DDR, SPEAKER); /* disable speaker output */
      }
    }
    

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



