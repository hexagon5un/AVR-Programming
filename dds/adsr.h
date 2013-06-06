
/*Include file for ADSR synth project. */

#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include <avr/interrupt.h>	
#include "pinDefines.h"
#include "macros.h"
#include "scale.h"
#include "fullSaw7.h"		/* a 7-harmonic bandlimited sawtooth */
                   /* see generateWavetables.py if you're interested */
#include "fullTriangle.h"
#include "UART.h"

#define FULL_VOLUME     31 	/* 5-bit volumes */

// Default envelope values (slightly percussive)
// Play around with these!  
#define ATTACK_RATE    8	/* 0-255 */
#define DECAY_RATE     120	/* 0-255 */
#define SUSTAIN_LEVEL  25	/* 0-255 */
#define SUSTAIN_TIME   4000	/* 0-65535 */
#define RELEASE_RATE   200	/* 0-65535 */

// Compute these constants
#define ATTACK_TIME   ATTACK_RATE * FULL_VOLUME
#define DECAY_TIME   (ATTACK_TIME + (FULL_VOLUME-SUSTAIN_LEVEL) * DECAY_RATE)
#define RELEASE_TIME     DECAY_TIME + SUSTAIN_TIME

// -------------- Init Routines --------------- //

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
}

static inline void initLEDs(void){
  uint8_t i;
  LED_DDR = 0xff;	/* All LEDs for diagnostics */
  for (i=0; i<8; i++){
    set_bit(LED_PORT, i);
    _delay_ms(100);
    clear_bit(LED_PORT, i);
  }
}

static inline uint16_t lookupPitch(char i){
  switch(i){
  case 'a':
    return(G1);
  case 's':
    return(A1);
  case 'd':
    return(B1);
  case 'f':
    return(C2);
  case 'g':
    return(D2);
  case 'h':
    return(E2);
  case 'j':
    return(F2);
  case 'k':
    return(G2);
  case 'l':
    return(A2);
  case ';':
    return(B2);
  case '\'':
    return(C3);   
  }	 
  // Default value -- if press some other key
  return(C1);
}

