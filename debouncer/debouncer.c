/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include "pinDefines.h"

#include <util/delay.h>             
#define DEBOUNCE_TIME  2	/* milliseconds */


uint8_t debounce(void){
  if (bit_is_clear(BUTTON_IN, BUTTON)){ /* button is pressed now */
    _delay_ms(DEBOUNCE_TIME); 
    if (bit_is_clear(BUTTON_IN, BUTTON)){ /* still pressed */
      return(1);
    }
  }
  return(0);
}

uint8_t checkButton(void){
  static uint8_t buttonWasPressed;
  if (debounce()){ 
    if (buttonWasPressed == 0){	  /* changed state? */
      buttonWasPressed = 1;
      return(1);
    }
  }
  else{
    buttonWasPressed = 0;
  }
  return(0);
}

int main(void){
  // -------- Inits --------- //
  BUTTON_PORT |= (1 << BUTTON);	/* enable the pullup on the button */
  LED_DDR = (1 << LED0);	/* set up LED for output */

  // ------ Event loop ------ //
  while(1){     
    if (checkButton()){ 
      LED_PORT ^= (1 << LED0);  /* do whatever */
    }	
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

