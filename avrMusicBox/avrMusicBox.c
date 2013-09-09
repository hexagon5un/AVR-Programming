/*

*/

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "organ.h"
#include "scale16.h"
#include "pinDefines.h"

#define NOTE_DURATION     0x2400        /* determines long note length */
#define DEBOUNCETIME      1  /* ms */ 

uint8_t debouncePoll(void){
	if (bit_is_clear(BUTTON_IN, BUTTON)){
		_delay_ms(DEBOUNCETIME);
		if (bit_is_clear(BUTTON_IN, BUTTON)){ 	/* button still pressed */
			return(1);
		}
	}
	return(0);
}

int main(void) {

	const uint16_t song[] = { C5, 
    D5,  E5, F5,  G5, 
    A5,  B5, C6
  };
	uint8_t whichNote = 0;
	uint8_t wasButtonPressed = 0; 
	
	// -------- Inits --------- //
  SPEAKER_DDR |= (1 << SPEAKER);                 /* speaker for output */
	BUTTON_PORT |= (1 << BUTTON);
  
	// ------ Event loop ------ //
  while (1) {
//		if (debouncePoll()){ /* pressed currently? */ 
		if (bit_is_clear(BUTTON_IN, BUTTON)){
			if (!wasButtonPressed){ /* if it's a new press ... */ 
				whichNote++; /* advance to next note */ 
				if (whichNote == sizeof(song)/2){ /* but don't run over the end */ 
					whichNote = 0;
				}
				wasButtonPressed = 1;
			}
			playNote(song[whichNote], NOTE_DURATION);
		}
		else{
			wasButtonPressed = 0;
		}
  }                                                  /* End event loop */
  return (0);
}
