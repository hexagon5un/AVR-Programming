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

  uint16_t i;

  // -------- Inits --------- //
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker for output */
  set_bit(BUTTON_PORT, BUTTON);	 /* button pullup */

  // ------ Event loop ------ //
  while(1){	
    
    while (bit_is_clear(BUTTON_IN, BUTTON)){

      playNote(C1, NOTE_DURATION/2);
      rest(NOTE_DURATION/2);
      playNote(E1, NOTE_DURATION/2);
      rest(NOTE_DURATION/2);
      playNote(G1, NOTE_DURATION/2);
      rest(NOTE_DURATION/2);
      playNote(C2, NOTE_DURATION/2);
      rest(NOTE_DURATION/2);
      playNote(C2, NOTE_DURATION);
      playNote(E2, NOTE_DURATION);
      playNote(G2, NOTE_DURATION);
      playNote(C3, NOTE_DURATION*2);

    }
  }   /* End event loop */
  return(0);			
}
