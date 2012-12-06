/* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */	

#include "pinDefines.h"
#include "macros.h"
#include "fullSine.h"
#include "USART.h"

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
}

static inline void pollButton(void){
  if (bit_is_clear(BUTTON_IN, BUTTON)) {
    set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */
    clear_bit(LED_PORT, LED0);
  }
  else {
    clear_bit(SPEAKER_DDR, SPEAKER); /* disable output on pin */
    set_bit(LED_PORT, LED0);
  }
}

static inline void initLED(void){
  set_bit(LED_DDR, LED0);	/* LED on for diagnostics */
  set_bit(LED_PORT, LED0);
  _delay_ms(100);
  clear_bit(LED_PORT, LED0);
}

int main(void){

  uint16_t accumulator;  
  uint16_t tuningWord;
  uint8_t  whichByte;
  uint8_t pwmValue;
  uint16_t i;

  // -------- Inits --------- //
  
  initLED();
  initTimer0();
  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  tuningWord = 800;		
  
  // ------ Event loop ------ //
  while(1){		       

    set_bit(SPEAKER_DDR, SPEAKER); /* connect speaker */

    for(i=0; i < 25600; i++){  // Play for 25,600 / 31,250 sec
     
      // Update the DDS 
      accumulator += tuningWord;	        /* take tuningWord steps */
      whichByte = (uint8_t) (accumulator >> 8);
      pwmValue = fullSine[whichByte];
      
      // Set PWM output
      loop_until_bit_is_set(TIFR0, TOV0); /* wait for overflow bit */
      OCR0A = 128 + pwmValue;	
      set_bit(TIFR0, TOV0);	        /* reset timer overflow bit */
      
      // And here we have about 28 microseconds left 
      //  (= 8*28 instructions) to do stuff 
      
    }
    
    clear_bit(SPEAKER_DDR, SPEAKER); /* disconnect speaker */
    _delay_ms(200);
    _delay_ms(200);
    _delay_ms(200);
	

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



