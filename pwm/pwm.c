/*  

Quick and dirty PWM Demo

*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"

#define LED_DELAY  2

int main(void){
  
  uint8_t brightness = 0;
  int8_t direction = 1;
  uint8_t i;

  // -------- Inits --------- //

  // Init LED 
  set_bit(LED_DDR, LED0);

  // ------ Event loop ------ //
  while(1){	
    
    // PWM
    for (i=0; i<255; i++){
      if (i < brightness){
	set_bit(LED_PORT, LED0);
      }
      else{
	clear_bit(LED_PORT, LED0);
      }
      _delay_us(20);
    }

    // Brighten and dim 
    if (brightness == 0){
      direction = 1;
    }
    if (brightness == 255){
      direction = -1;
    }
    brightness += direction;


  } /* End event loop */
  return(0);		      /* This line is never reached  */
}




