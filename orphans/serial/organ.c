/*

Simple routines to play notes out to a speaker

*/

#include <avr/io.h>
#include <util/delay.h>
#include "organ.h"
#include "pinDefines.h"

void playNote(uint16_t wavelength, uint16_t duration) {
  uint16_t elapsed;
  uint16_t i;
  for (elapsed = 0; elapsed < duration; elapsed += wavelength) {
                     /* For loop with variable delay selects the pitch */
    for (i = 0; i < wavelength; i++) {
      asm volatile ("nop"::);
      /* Note that doing _something_ is necessary here,
         or else the compiler will notice that nothing is happening
         and optimize the delay away. */
    }
    SPEAKER_PORT ^= (1 << SPEAKER);
  }
}

void rest(uint16_t duration) {
  do {
    asm volatile ("nop"::);
  } while (--duration);
}
