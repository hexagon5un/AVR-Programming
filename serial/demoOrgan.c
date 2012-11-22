/* 

demoOrgan.c

Plays a song when you press the button.
See organ.h for pin defines and other macros
See organ.c (and include it in the Makefile) for playNote() and rest()

*/


// ------- Preamble -------- //
#include <avr/io.h>		
#include <util/delay.h>		
#include "organ.h"
#include "scale16.h"

int main(void){

  // -------- Inits --------- //
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker for output */
  set_bit(BUTTON_PORT, BUTTON);	 /* button pullup */

  // ------ Event loop ------ //
  while(1){	
    
    if (bit_is_clear(BUTTON_IN, BUTTON)){
  
      playNote(E2, NOTE_DURATION/2 - NOTE_DURATION/32);
      rest(NOTE_DURATION/32);	
      /* slight pause btw notes makes less legato */
      playNote(E2, NOTE_DURATION/2);
      rest(NOTE_DURATION/2);
      playNote(E2, NOTE_DURATION/2);
      rest(NOTE_DURATION/2);
      playNote(C2, NOTE_DURATION/2);
      playNote(E2, NOTE_DURATION/2);
      rest(NOTE_DURATION/2);	
      // bar
      playNote(G2, NOTE_DURATION);
      rest(NOTE_DURATION);
      rest(NOTE_DURATION/2);
      playNote(G1, NOTE_DURATION);      

    }

  }   /* End event loop */
  return(0);			
}
