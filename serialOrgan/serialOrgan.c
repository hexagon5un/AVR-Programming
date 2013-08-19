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
#include "pinDefines.h"
#include "USART.h"

#define NOTE_DURATION     0xF000        /* determines long note length */

int main(void) {

  // -------- Inits --------- //
  SPEAKER_DDR |= (1 << SPEAKER);                 /* speaker for output */
  initUSART();
  printString("----- Serial Organ ------\r\n");

  char fromCompy;                        /* used to store serial input */
  uint16_t currentNoteLength = NOTE_DURATION / 2;
  const uint8_t keys[] = { 'a', 'w', 's', 'e', 'd', 'f', 't',
    'g', 'y', 'h', 'j', 'i', 'k', 'o',
    'l', 'p', ';', '\''
  };
  const uint16_t notes[] = { G4, Gx4, A4, Ax4, B4, C5, Cx5,
    D5, Dx5, E5, F5, Fx5, G5, Gx5,
    A5, Ax5, B5, C6
  };
  uint8_t isNote;
  uint8_t i;

  // ------ Event loop ------ //
  while (1) {

                                                            /* Get Key */
    fromCompy = receiveByte();      /* waits here until there is input */
    transmitByte('N');     /* alert computer we're ready for next note */

                                                         /* Play Notes */
    isNote = 0;
    for (i = 0; i < sizeof(keys); i++) {
      if (fromCompy == keys[i]) {       /* found match in lookup table */
        playNote(notes[i], currentNoteLength);
        isNote = 1;                  /* record that we've found a note */
        break;                               /* drop out of for() loop */
      }
    }

                      /* Handle non-note keys: tempo changes and rests */
    if (!isNote) {
      if (fromCompy == '[') {                   /* code for short note */
        currentNoteLength = NOTE_DURATION / 2;
      }
      else if (fromCompy == ']') {               /* code for long note */
        currentNoteLength = NOTE_DURATION;
      }
      else {                                /* unrecognized, just rest */
        rest(currentNoteLength);
      }
    }

  }                                                  /* End event loop */
  return (0);
}
