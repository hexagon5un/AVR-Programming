/* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */	

#include "pinDefines.h"
#include "macros.h"
#include "fullSine.h"

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
}

int main(void){

  uint16_t accumulator;  
  uint16_t accumulatorSteps = 880; /* approx 440 Hz */
  uint8_t  waveStep;
  int8_t   pwmValue;

  // -------- Inits --------- //
  
  initTimer0();
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  
  // ------ Event loop ------ //
  while(1){		       

    if (bit_is_clear(BUTTON_IN, BUTTON)){

      set_bit(SPEAKER_DDR, SPEAKER);    /* enable speaker */
      accumulator += accumulatorSteps;  /* advance accumulator */
      waveStep = accumulator >> 8;      /* which entry in lookup? */
      pwmValue = fullSine[waveStep];    /* lookup voltage */
      
      loop_until_bit_is_set(TIFR0, TOV0);  /* wait for PWM cycle */
      OCR0A = 128 + pwmValue;		   /* set new PWM value */
      set_bit(TIFR0, TOV0);	           /* reset PWM overflow bit */
    }

    else{			       /* button not pressed */
      clear_bit(SPEAKER_DDR, SPEAKER); /* disable speaker */
    }

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



