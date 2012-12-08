/*  

Cross-fading Cylon Eyes 
A PWM demo

*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"

int main(void){
  
  uint8_t thisEye = 0;
  uint8_t nextEye = 1;
  uint8_t brightness = 0;
  int8_t direction = 1;
  uint8_t i;

  // -------- Inits --------- //

  // Init all LEDs 
  LED_DDR = 0xff;	
  set_bit(SPEAKER_DDR, SPEAKER);
  // ------ Event loop ------ //
  while(1){	
    
    // PWM
    for (brightness=0; brightness<255; brightness++){
      for (i=0; i<255; i++){
	if (i < brightness){
	  clear_bit(LED_PORT, thisEye); /* dimming */
	  set_bit(LED_PORT, nextEye); /* getting brighter */
	}
	else{
	  set_bit(LED_PORT, thisEye); 
	  clear_bit(LED_PORT, nextEye); 
	}
	clear_bit(LED_PORT, thisEye); /* make sure it's off */
      }      
   }

    // Cylon Scan
    thisEye += direction;	/* move on to next eye */

    if (thisEye == 0){
      direction = 1;
    }
    if (thisEye == 7){
      direction = -1;
    }

    nextEye = thisEye + direction;


  } /* End event loop */
  return(0);		      /* This line is never reached  */
}




