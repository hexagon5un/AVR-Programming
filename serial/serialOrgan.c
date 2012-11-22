/* 

serialOrgan.c

Reads a character in serial from the keyboard, plays a note.

See organ.h for pin defines and other macros
See organ.c (and include it in the Makefile) for playNote() and rest()

*/


// ------- Preamble -------- //
#include <avr/io.h>		
#include <util/delay.h>		
#include "organ.h"
#include "scale16.h"
#define BAUDRATE   9600
#include "USART.h"

int main(void){

  // -------- Inits --------- //
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker for output */
  set_bit(BUTTON_PORT, BUTTON);	 /* button pullup */
  
  initUSART();
  sayOK();
 
  char fromCompy;
  uint16_t currentNoteLength = NOTE_DURATION/2;   

  // ------ Event loop ------ //
  while(1){	

    /* Get Note */
    fromCompy = receiveByte();

    /* Play Note */
    switch(fromCompy){
    case 'a':
      playNote(G1, currentNoteLength);
      break;
    case 's':
      playNote(A1, currentNoteLength);
      break;
    case 'd':
      playNote(B1, currentNoteLength);
      break;
    case 'f':
      playNote(C2, currentNoteLength);
      break;
    case 'g':
      playNote(D2, currentNoteLength);
      break;
    case 'h':
      playNote(E2, currentNoteLength);
      break;
    case 'j':
      playNote(F2, currentNoteLength);
      break;
    case 'k':
      playNote(G2, currentNoteLength);
      break;
    case 'l':
      playNote(A2, currentNoteLength);
      break;
    case ';':
      playNote(B2, currentNoteLength);
      break;
    case '\'':
      playNote(C3, currentNoteLength);
      break;
    // Rests, and changing note length
    case ' ':
      rest(currentNoteLength);
      break;
    case '[':
      currentNoteLength = NOTE_DURATION/2;
      break;
    case ']':
      currentNoteLength = NOTE_DURATION;
      break;
    }
    

  }   /* End event loop */
  return(0);			
}





