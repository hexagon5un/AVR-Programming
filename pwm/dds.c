/* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"
#include "fullWaves.h"
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

int main(void){

  // -------- Inits --------- //
  uint16_t accumulator;  
  uint16_t tuningWord;
  uint8_t lookupByte;

  initTimer0();
  initUSART();
 
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */

  set_bit(LED_DDR, LED0);	/* LED on for diagnostics */
  set_bit(LED_PORT, LED0);
  _delay_ms(100);
  clear_bit(LED_PORT, LED0);
   
  set_bit(SPEAKER_DDR, SPEAKER);
  
  tuningWord = 16000;
  //tuningWord = 1;

  // ------ Event loop ------ //
  while(1){		       
    
    accumulator += tuningWord;	/* take tuningWord steps forward */
    lookupByte = (uint8_t) (accumulator >> 8);
    OCR0A = fullSine[lookupByte];
    //    _delay_us(32);		/* let it get through full cycle */
    // ?? am I messing up with cycle timing?  Do this with interrupts???
    transmitByte(fullSine[lookupByte]);
    
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



