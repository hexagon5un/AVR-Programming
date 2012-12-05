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
#define ATTACK_TIME    (FULL_VOLUME * ATTACK_RATE)
#define SUSTAIN_TIME   (ATTACK_TIME + 6000)
#define RELEASE_RATE   400

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

  uint16_t accumulator = 0;  
  uint8_t  volume = 0;
  uint16_t clock  = 0;
  uint16_t tuningWord = C2;    /* change everywhere to pitch */
  uint8_t waveStep;
  int8_t PWM;
  uint8_t i;

  // -------- Inits --------- //
  
  initLEDs();
  initTimer0();
  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  // ------ Event loop ------ //
  while(1){		       

    set_bit(LED_PORT, LED6);		/* debugging -- begins wait time */
    loop_until_bit_is_set(TIFR0, TOV0); /* wait for timer0 overflow */
    clear_bit(LED_PORT, LED6);		/* debugging -- ends wait time */
    accumulator += tuningWord;
    waveStep = (uint8_t) (accumulator >> 8);
    PWM = (fullTriangle[waveStep] * volume) >> 5;
    OCR0A = 128 + PWM; 		/* int8_t to uint8_t */
    set_bit(TIFR0, TOV0);		/* reset the overflow bit */

    
    if (clock){		/* if clock already running */
      clock++;
      /* if (!(clock % 15)){ */
      /* 	set_bit(LED_PORT, LED2); */
      /* } */
      if (clock < ATTACK_TIME) {
	if (clock > ATTACK_RATE*volume){
	  if (volume < 31){
	    volume++;
	  }
	}
      }
      else {
	if (clock > SUSTAIN_TIME){ 			/* in release phase */
	  if ((clock - SUSTAIN_TIME) > (31-volume)*RELEASE_RATE){
	    if (volume > 0){
	      volume--;
	    }
	    else{
	      clock = 0;
	    }
	  }
	}
      }
    }
    else{				    /* if not in clock loop, check button */
      if (bit_is_clear(BUTTON_IN, BUTTON)){ /* if pressed, start clock */
	clock = 1;
      }
    }
      
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}

