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
#include "USART.h"

#define FULL_VOLUME     31 	/* 5-bit volumes */

#define ATTACK_RATE    3
#define ATTACK_TIME    (FULL_VOLUME * ATTACK_RATE)
#define DECAY_RATE     50

#define SUSTAIN_LEVEL  20
#define DECAY_TIME     (ATTACK_TIME + (FULL_VOLUME-SUSTAIN_LEVEL) * DECAY_RATE)

#define SUSTAIN_TIME   2000
#define RELEASE_TIME   (DECAY_TIME + SUSTAIN_TIME)

#define RELEASE_RATE   200

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

  uint16_t accumulator = 0;  
  uint8_t  volume = 0;
  uint16_t clock  = 0;
  uint16_t tuningWord = C1;    
  uint8_t waveStep;
  int8_t PWM;
  uint8_t i;
  uint8_t buttonPressed;

  // -------- Inits --------- //
  
  initLEDs();
  initTimer0();
  initUSART();
  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  // ------ Event loop ------ //
  while(1){		       

    set_bit(LED_PORT, LED0);		/* debugging -- begins wait time */
    loop_until_bit_is_set(TIFR0, TOV0); /* wait for timer0 overflow */
    clear_bit(LED_PORT, LED0);		/* debugging -- ends wait time */
    accumulator += tuningWord;
    waveStep = (uint8_t) (accumulator >> 8);
    PWM = (fullSine[waveStep] * volume) >> 5;
    OCR0A = 128 + PWM; 		/* int8_t to uint8_t */
    set_bit(TIFR0, TOV0);		/* reset the overflow bit */
    
    if (clock){		     /* if clock already running */
      clock++;
      if (clock < ATTACK_TIME) { /* attack */
	if (clock > ATTACK_RATE*volume){
	  if (volume < 31){
	    volume++;
	  }
	}
      }
      else if (clock < DECAY_TIME) {  /* decay */
	if ((clock - ATTACK_TIME) > (FULL_VOLUME-volume)*DECAY_RATE){
	  if (volume > SUSTAIN_LEVEL){
	    volume--;
	  }
	}
      }
      else if (clock > RELEASE_TIME){  /* release  */
	if ((clock - RELEASE_TIME) > (SUSTAIN_LEVEL-volume)*RELEASE_RATE){
	  if (volume > 0){
	    volume--;
	  }
	  else{
	    clock = 0;
	  }
	}
      }
    }
    else{				    /* if not in clock loop, check button */
      if (bit_is_clear(BUTTON_IN, BUTTON)){ 
	/* first press, start clock */
	if (!buttonPressed){
	  clock = 1;
	  buttonPressed = 1;
	  set_bit(LED_PORT, LED7);	
	}
      }
      else{
	buttonPressed = 0;
	clear_bit(LED_PORT, LED7);
      }
    }
    
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}
