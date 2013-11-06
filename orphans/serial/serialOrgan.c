/*

serialOrgan.c

Reads a character in serial from the keyboard, plays a note.

See organ.h for pin defines and other macros
See organ.c (and include it in the Makefile) for playNote() and rest()

*/


// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "organ.h"
#include "scale16.h"
#include "USART.h"

int main(void) {

  // -------- Inits --------- //
  SPEAKER_DDR |= (1 << SPEAKER);                 /* speaker for output */
  BUTTON_PORT |= (1 << BUTTON);                       /* button pullup */

  initUSART();
  printString("----- Serial Organ ------\r\n");

  char fromCompy;
  uint16_t currentNoteLength = NOTE_DURATION / 2;

  // ------ Event loop ------ //
  while (1) {

                                                           /* Get Note */
    fromCompy = receiveByte();
    transmitByte('N');     /* tells computer we're ready for next note */

                                                         /* Play Notes */
    switch (fromCompy) {
    case 'a':                               /* when typed 'a', play G1 */
      playNote(G1, currentNoteLength);
      break;
    case 's':                                                   /* etc */
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
    case '[':                                   /* code for short note */
      currentNoteLength = NOTE_DURATION / 2;
      break;
    case ']':                                    /* code for long note */
      currentNoteLength = NOTE_DURATION;
      break;
    default:                                 /* doesn't match anything */
      rest(currentNoteLength);
      break;
    }


  }                                                  /* End event loop */
  return (0);
}
