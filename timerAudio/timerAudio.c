/*  

Quick audio demo using Timer 0 to generate audio frequencies directly.

*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"
#include "scale8.h"		/* 8-bit scale */

static inline void initTimer(void){
  set_bit(TCCR0A, WGM01);	/* CTC mode */
  set_bit(TCCR0A, COM0A0);	/* Toggles pin each cycle through */
  set_bit(TCCR0B, CS00);
  set_bit(TCCR0B, CS01);        /* CPU clock / 64 */
}

static inline void playNote(uint8_t wavelength, uint16_t duration){

  OCR0A = wavelength;		/* set pitch */
  set_bit(SPEAKER_DDR, SPEAKER);/* enable output on speaker */

  while(duration){	      	/* Variable delay */
    _delay_ms(1);
    duration--;
  }
  clear_bit(SPEAKER_DDR, SPEAKER); /* turn speaker off */
}

int main(void){
  // -------- Inits --------- //
  initTimer();
  // ------ Event loop ------ //
  while(1){	    
    /* Play some notes */
    playNote(C2, 200);
    playNote(E2, 200);
    playNote(G2, 200);
    playNote(C3, 400);

    _delay_ms(1000);
    _delay_ms(1000);
    _delay_ms(1000);
    
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}

