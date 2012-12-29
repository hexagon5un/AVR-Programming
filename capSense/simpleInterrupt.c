/* 

Demo of using interrupts for doing what they do best -- 
two things at once.

Flashes LED0 at a fixed rate, interrupting whenever button is pressed.

 */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define DEBOUNCE_TIME 2 	/* milliseconds */
uint8_t debounceButton(void);	/* Quick and dirty debounce routine */

ISR(INT0_vect){ 		/* Run every time there is a change on button */
  if (debounceButton()){
    set_bit(LED_PORT, LED7);
  }
  else{
    clear_bit(LED_PORT, LED7);
  }
}

void initInterrupt0(void){
  set_bit(EIMSK, INT0);	       /* enable INT0 */
  set_bit(EICRA, ISC00);       /* trigger when button changes state */
  sei();		       /* set (global) interrupt enable bit */
}

int main(void){
  // -------- Inits --------- //
  set_bit(LED_DDR, LED0);
  set_bit(LED_DDR, LED7);
  set_bit(BUTTON_PORT, BUTTON);	/* pullup */
  initInterrupt0();
  
  // ------ Event loop ------ //
  while(1){	

    // Blink LED0 forever
    _delay_ms(100);
    set_bit(LED_PORT, LED0);
    _delay_ms(100);
    clear_bit(LED_PORT, LED0);

  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}

uint8_t debounceButton(void){
  /* Simple debounce */
  if (bit_is_clear(BUTTON_IN, BUTTON)){ /* pressed? */
    _delay_ms(DEBOUNCE_TIME);			
    if (bit_is_clear(BUTTON_IN, BUTTON)){ /* still pressed? */
      return(1);
    }
  }
  else{				/* un-debounced button-release */
    return(0);
  }  
}
