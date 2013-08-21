/*
 *Using two light sensors (pitch and volume),
 *and our previous PWM DDS audio routines,
 *plus a whole ton of ISRs,
 *we can make a fairly decent-sounding Theremin-like instrument.
 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "fullTri7.h"

// -------- Global Variables --------- //
// -- For direct-digital synthesis --- //

// -------- Functions --------- //

static inline void initTimer0(void);
ISR(TIMER0_OVF_vect); 
static inline void initADC(void);
ISR(ADC_vect);

int main(void) {

  // -------- Inits --------- //

  initTimer0();
  SPEAKER_DDR |= (1 << SPEAKER);           /* enable output to speaker */

  initADC();
  sei();                                      /* Enable all interrupts */
  ADCSRA |= (1 << ADSC);                          /* start conversions */

  // ------ Event loop ------ //

  while (1) {
    ; 
  }
  return (0);
}
