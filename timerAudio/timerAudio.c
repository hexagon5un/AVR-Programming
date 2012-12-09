/*  

Quick audio demo using Timer 0 to generate audio frequencies directly.

*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"
#include "scale8.h"		/* 8-bit scale */

int main(void){
  // -------- Inits --------- //

  set_bit(TCCR0A, WGM01);	/* CTC mode */
  set_bit(TCCR0A, COM0A0);	/* Toggles pin each cycle through */
  TCCR0B |= ((1 << CS01) | (1 << CS00));        /* CPU clock / 64 */
  
  // ------ Event loop ------ //
  while(1){	
    
    set_bit(SPEAKER_DDR, SPEAKER); /* enable output on speaker */

    /* Play some notes */
    OCR0A = C2;
    _delay_ms(200);
    OCR0A = E2;
    _delay_ms(200);
    OCR0A = G2;
    _delay_ms(200);
    OCR0A = C3;
    _delay_ms(400);
    
    clear_bit(SPEAKER_DDR, SPEAKER); /* turn speaker off */

    _delay_ms(2000);
    _delay_ms(2000);
    _delay_ms(2000);
    
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}

