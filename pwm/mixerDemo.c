
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
#include "fullSine.h"
#include "scale.h"

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

  uint16_t accumulators[4] = {0,0,0,0};  
  uint16_t tuningWords[4] = {C2, E2, G2, C3}; // Notes are a C Maj chord
  uint8_t  volumes[4] = {0,0,0,0};
  uint16_t mixer;

  uint8_t waveStep;
  uint8_t i;
  uint16_t cycleCount = 0;

  // -------- Inits --------- //
  
  initLEDs();
  initTimer0();
  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  // ------ Event loop ------ //
  while(1){		       
    
    // Update PWM
    loop_until_bit_is_set(TIFR0, TOV0); /* wait for timer0 overflow */
    set_bit(TIFR0, TOV0);		/* reset the overflow bit */
    OCR0A = 128 + mixer;		/* update the value */

    /* Update all accumulators, mix together */
    mixer = 0;
    for (i=0; i < 4; i++){
      accumulators[i] += tuningWords[i]; /* accumulator update */
      waveStep = (uint8_t) (accumulators[i] >> 8); 
      mixer += fullSine[waveStep] * volumes[i];
    }
    mixer = mixer >> 5;		/* 5-bit volume */
    mixer = mixer >> 2;		/* quick divide by 4 voices */

    cycleCount++;
    LED_PORT = volumes[0];
    volumes[0] = cycleCount >> 11;
    volumes[1] = (cycleCount - 10000 ) >> 11;
    volumes[2] = (cycleCount - 20000 ) >> 11;
    volumes[3] = (cycleCount - 30000 ) >> 11;
    
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



