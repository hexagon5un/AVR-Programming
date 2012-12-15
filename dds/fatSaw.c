
/* 
   Direct-digital synthesis
   Phasing saw waves demo
   
*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include <avr/interrupt.h>	
#include "pinDefines.h"
#include "macros.h"
#include "fullSaw15.h"

#define BASEPITCH       220 /* in tuningWord steps, which are ~1/2 Hz */
#define PHASE_RATE      10 /* control speed of phasing effect */

#define NUMBER_OSCILLATORS  2  	
/* 2 and 4 work just fine.
   8 and 16 take too long to maintain our 31.25kHz sample rate
   so the pitch shifts downwards and there's all sorts of aliasing.  
   If you're just after scary sounds, 8 and 16 are awesome. */
#define  OSCILLATOR_SHIFT   1
/* This is the number of bits to shift: 
   2**OSCILLATOR_SHIFT = NUMBER_OSCILLATORS 
   If you don't change this to match the number of oscillators, you'll
   get clipping and digital distortion, which is ugly.  */

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
}


int main(void){

  uint16_t accumulators[NUMBER_OSCILLATORS];  
  uint16_t tuningWords[NUMBER_OSCILLATORS];
  uint8_t waveStep;
  int16_t mixer;
  uint8_t i;
  

  // -------- Inits --------- //
  
  initTimer0();  
  set_bit(LED_DDR, LED0);

  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */

  // Start same pitch, phase
  for (i=0; i<NUMBER_OSCILLATORS; i++){
    tuningWords[i] = BASEPITCH;
    accumulators[i] = 0;	
  }

  // ------ Event loop ------ //
  while(1){		       
    
    /* Load in the PWM value when ready */
    loop_until_bit_is_set(TIFR0, TOV0); /* wait until overflow bit set */
    OCR0A = 128 + mixer;		/* signed-integers need shifting up */
    set_bit(TIFR0, TOV0);		/* re-set the overflow bit */

    /* Update all accumulators, mix together */
    mixer = 0;
    for (i=0; i<NUMBER_OSCILLATORS; i++){
      accumulators[i] += tuningWords[i];
      waveStep = accumulators[i] >> 8;
      mixer += fullSaw15[waveStep]; 
    }
    mixer = mixer >> OSCILLATOR_SHIFT;	  
    /* Dividing by bitshift is very fast.*/

    // Add extra phase increment to some waves 
    // makes crazy shifting overtones
    for (i=1; i<NUMBER_OSCILLATORS; i++){
      if (accumulators[i] < tuningWords[i]){       /* once per cycle */
	accumulators[i] += PHASE_RATE*i; /* add extra phase */
      }
    }

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



