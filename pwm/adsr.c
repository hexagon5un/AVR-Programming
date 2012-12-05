/* 
   Direct-digital synthesis
   ADSR Demo
   
*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include <avr/interrupt.h>	
#include "pinDefines.h"
#include "macros.h"
#include "scale.h"
#include "fullWaves.h"


#define FULL_VOLUME     31 	/* 5-bit volumes */

#define ATTACK_RATE    100
#define ATTACK_TIME    FULL_VOLUME * ATTACK_RATE
#define DECAY_RATE     200
#define DECAY_TIME     FULL_VOLUME * DECAY_RATE + ATTACK_TIME

#define SUSTAIN_VALUE   28
#define SUSTAIN_TIME    30000 + DECAY_TIME
#define RELEASE_RATE    800

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

// Note: signed integer here: -128 to 127
static inline int8_t triangle(uint8_t waveStep){
  int8_t triangleValue;
  if (waveStep < 64){          /* 0..63 -> 1..127 */
    triangleValue = (2*waveStep)+1;
  }
  else if (waveStep < 192){    /* 64..191 -> 126..-128 */
    triangleValue =  126 - 2*(waveStep - 64);
  }
  else {                       /* 192..255 -> -127..-1 */
    triangleValue =  -127 + 2*(waveStep - 192);
  }
  return(triangleValue);
}

int main(void){

  uint16_t accumulators[2] = {0,0};  
  uint8_t  volumes[2] = {0,0};
  uint16_t clocks[2]  =  {0,1};
  uint16_t tuningWords[2];    /* change everywhere to pitch */
  /* signed output makes math easier */
  int16_t mixer;

  uint8_t waveStep;
  uint8_t i;

  // -------- Inits --------- //
  
  initLEDs();
  initTimer0();
  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  // Notes are a C Maj chord
  tuningWords[0] = G2;
  tuningWords[1] = C3;
  
  // ------ Event loop ------ //
  while(1){		       

    loop_until_bit_is_set(TIFR0, TOV0); /* wait for timer0 overflow */

    set_bit(TIFR0, TOV0);		/* reset the overflow bit */
    OCR0A = 128 + mixer;			/* update the value */

    /* Update all accumulators, mix together */
    
    mixer = 0;
    for (i=0; i < 2; i++){
      accumulators[i] += tuningWords[i]; /* accumulator update */
      waveStep = (uint8_t) (accumulators[i] >> 8); 
      // Triangle wave in code: costs about 5 us, but it's variable
      // It could probably be waaaay streamlined.
      //mixer += triangle(waveStep) * volumes[i]; /* function call */
      mixer += fullTriangle[waveStep] * volumes[i];  /* lookup table */
    }
    mixer = mixer >> 5;		/* 5-bit volume */
    mixer = mixer >> 1;		/* quick divide by 4 voices */
    
    set_bit(LED_PORT, LED0);		/* debugging -- begins wait time */
    if (clocks[0]){		/* if clock already running */
      clocks[0]++;
      LED_PORT = clocks[0]>>8;
      if (clocks[0] < ATTACK_TIME){
	if (!(clocks[0] % ATTACK_RATE) && (volumes[0] < 31)){
	  volumes[0]++;
	}
      }
      else if (clocks[0] < DECAY_TIME){
	if (!(clocks[0] % DECAY_RATE) && (volumes[0] > SUSTAIN_VALUE)){
	  volumes[0]-- ;
	}
      }
      else if (clocks[0] > SUSTAIN_TIME){
	if (!(clocks[0] % RELEASE_RATE) && (volumes[0] > 0)){
	  volumes[0]-- ;
	}
      }
     
    }
    else{				    /* if not in clock loop, check button */
      if (bit_is_clear(BUTTON_IN, BUTTON)){ /* if pressed, start clock */
	  clocks[0] = 1;
      }
    }
    clear_bit(LED_PORT, LED0);		/* debugging -- ends wait time */

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}


/* 

#define FULL_VOLUME     31 	

#define ATTACK_RATE    10
#define ATTACK_TIME    FULL_VOLUME * ATTACK_RATE
#define DECAY_RATE     20
#define DECAY_TIME     FULL_VOLUME * DECAY_RATE + ATTACK_TIME

#define SUSTAIN_VALUE   28
#define SUSTAIN_TIME    8000 + DECAY_TIME
#define RELEASE_RATE    80
#define ATTACK_TIME     200 


*/
