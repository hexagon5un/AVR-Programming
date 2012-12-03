
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
#include "fullWaves.h"
#include "scale.h"
#include "USART.h"

#define BASEPITCH       800 /* in tuningWord steps, roughly 1/2 Hz */
#define SPEED           32   /* powers of two are good values. */

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

  uint16_t accumulators[4];  
  uint16_t tuningWords[4];
  uint8_t  volumes[4];

  uint16_t mixer;
  uint8_t PWM_value;
  uint8_t i;
  

  // -------- Inits --------- //
  
  initLEDs();
  initTimer0();
  initUSART();
  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  // Notes are a C Maj chord
  tuningWords[0] = C1;
  tuningWords[1] = E1;
  tuningWords[2] = G1;
  tuningWords[3] = C2;
  
  volumes[0] = 0;		/* 5-bit volumes, 32 is max */
  volumes[1] = 0;		/* 5-bit volumes, 32 is max  */
  volumes[2] = 0;		/* 5-bit volumes, 32 is max  */
  volumes[3] = 0;		/* 5-bit volumes, 32 is max  */

  // ------ Event loop ------ //
  while(1){		       
    
    loop_until_bit_is_set(TIFR0, TOV0); /* wait for timer0 overflow */
    set_bit(TIFR0, TOV0);		/* reset the overflow bit */
    OCR0A = mixer;			/* update the value */

    /* Update all accumulators, mix together */
    mixer = 0;
    for (i=0; i < 4; i++){
      accumulators[i] += tuningWords[i]; /* accumulator update */
      PWM_value = fullSine[(uint8_t) (accumulators[i] >> 8)]; // sine wave version
      // PWM_value = (uint8_t) (accumulators[i] >> 8); // sawtooth version
      // PWM_value = fullTriangle[(uint8_t) (accumulators[i] >> 8)];  // triangle version
      mixer += PWM_value * volumes[i];
    }
    mixer = mixer >> 5;		/* quick divide by 5 bit volume */
    mixer = mixer >> 2;		/* quick divide by 4 voices */
    LED_PORT = (LED_PORT + mixer) >> 1;
    

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



