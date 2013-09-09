/* Music Box Input Demo */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "organ.h"
#include "scale16.h"
#include "pinDefines.h"

int main(void) {
	const uint16_t song[] = { 
		E6, E6, E6, C6, E6, G6, G5, 
		C6, G5, E5, A5, B5, Ax5, A5, 
		G5, E6, G6, A6, F6, G6, E6, C6, D6, B5, 
		C6, G5, E5, A5, B5, Ax5, A5, 
		G5, E6, G6, A6, F6, G6, E6, C6, D6, B5,
		/* etc */
	};
	uint8_t whichNote = sizeof(song)/2 -1;
	uint8_t wasButtonPressed = 0; 
	
	// -------- Inits --------- //
  SPEAKER_DDR |= (1 << SPEAKER);                 /* speaker for output */
	BUTTON_PORT |= (1 << BUTTON); /* pullup on button */ 
  
	// ------ Event loop ------ //
  while (1) {
		if (bit_is_clear(BUTTON_IN, BUTTON)){
			if (!wasButtonPressed){ /* if it's a new press ... */ 
				whichNote++; /* advance to next note */ 
				if (whichNote == sizeof(song)/sizeof(uint16_t)){ /* but don't run over the end */ 
					whichNote = 0;
				}
				wasButtonPressed = 1;
			}	
			playNote(song[whichNote], 1600);
		}
		else{
			wasButtonPressed = 0;
		}
  }                                                  /* End event loop */
  return (0);
}
