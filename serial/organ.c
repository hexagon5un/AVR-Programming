/* 


*/

#include <avr/io.h>		
#include <util/delay.h>		
#include "organ.h"

void playNote(uint16_t wavelength, uint16_t duration){
  uint16_t cycleCount;
  uint16_t i;
  for(cycleCount = 0; cycleCount < duration; cycleCount += wavelength){
    /* For loop with variable delay selects the pitch */
    for (i = 0; i < wavelength; i++){ 
      asm volatile("nop"::);
    }
    toggle_bit(SPEAKER_PORT, SPEAKER);
  }
}

void rest(uint16_t duration){
  uint8_t i;
  do { 
    asm volatile("nop"::); 
  } while(--duration);
}
