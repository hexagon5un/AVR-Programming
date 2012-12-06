
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
#include "fullSaw7.h"
#include "USART.h"

#define BASEPITCH       220 /* in tuningWord steps, which are roughly 1/2 Hz */
#define PHASE_RATE      10 /* control speed of phasing effect */

#define NUMBER_OSCILLATORS  2  	
/* 2 and 4 work just fine.
   8 and 16 take too long to maintain our 31.25kHz sample rate
   and there's all sorts of aliasing and etc.  They are not
   "accurate", but if you're just after scary sounds, 16 is awesome.
   Powers of two are good choices here because of bit-shift divide.
   For example, it takes longer to run with 3 oscillators than 4.
*/


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
  uint8_t  i;
  

  // -------- Inits --------- //
  
  initTimer0();
  initUSART();
  
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
    
    set_bit(LED_PORT, LED0);
    loop_until_bit_is_set(TIFR0, TOV0); /* wait until overflow bit set */
    clear_bit(LED_PORT, LED0);
    OCR0A = 128 + mixer;		/* signed-integers need shifting up */
    set_bit(TIFR0, TOV0);		/* re-set the overflow bit */

    /* Update all accumulators, mix together */
    mixer = 0;
    for (i=0; i<NUMBER_OSCILLATORS; i++){
      accumulators[i] += tuningWords[i];
      waveStep = accumulators[i] >> 8;
      mixer += fullSaw7[waveStep]; // 7-partial sawtooth
    }
    mixer = mixer / NUMBER_OSCILLATORS;	  
    // mixer = mixer >> 2;	  
    /* As long as NUMBER_OSCILLATORS is a power of two, 
       the compiler will use a bit-shift to divide.  
       Other choices may take too long. */

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



