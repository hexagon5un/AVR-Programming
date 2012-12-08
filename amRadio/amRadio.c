/*  



*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"

#define COUNTER_VALUE   3  	/* determines carrier frequency */
// From f = f_cpu / ( 2* N* (1 + OCRnx) )
// Good values for the AM band from 2 to 8: pick one that's clear
// (1+OCRnx) in denominator b/c counts up from 0 -- 0, 1, 2, 3

// Divide by two b/c we're toggling on or off each loop; 
//  a full cycle of the carrier takes two loops.

// These are theoretical values, my chip apparently runs faster
//  and they measure a little higher
// 8Mhz / (2 * 1 * (1+3)) = 1000 kHz
// 8Mhz / (2 * 1 * (1+4)) = 800 kHz
// 8Mhz / (2 * 1 * (1+5)) = 670 kHz
// 8Mhz / (2 * 1 * (1+7)) = 500 kHz


static inline void initTimer(void){

  set_bit(TCCR0A, WGM01);	/* CTC mode */
  set_bit(TCCR0A, COM0B0);	/* Toggles pin each time through */
  set_bit(TCCR0B, CS00);	/* Clock at CPU frequency, ~8MHz */

  OCR0A = COUNTER_VALUE;	/* carrier frequency */
}

int main(void){
  
  uint8_t i;

  // -------- Inits --------- //
  initTimer();
  
  // ------ Event loop ------ //
  while(1){	

    // Turn on the transmitter: full power
    set_bit(ANTENNA_DDR, ANTENNA);
    _delay_ms(2);

    // Turn off the transmitter: no power
    clear_bit(ANTENNA_DDR, ANTENNA);
    _delay_ms(2);
    
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}




