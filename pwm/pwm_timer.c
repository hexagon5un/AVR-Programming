/*  

Quick and dirty PWM Demo

*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);      /* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);       /* Fast PWM mode */
  set_bit(TCCR0A, WGM01);       /* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);        /* Clock with /1 prescaler */
}

int main(void){
  
  int8_t direction = 1;

  // -------- Inits --------- //

  initTimer0();
  // Init LED 
  set_bit(LED_DDR, LED0);

  // ------ Event loop ------ //
  while(1){	

    // Brighten and dim 
    if (OCR0A == 0){
      direction = 1;
    }
    if (OCR0A == 255){
      direction = -1;
    }
    OCR0A += direction;
    _delay_ms(2);

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}




