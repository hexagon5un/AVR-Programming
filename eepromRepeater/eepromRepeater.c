/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "pinDefines.h"
#include "macros.h"
#include "UART.h"
#include "scale8.h"             /* 8-bit scale */


// -------- Functions --------- //
static inline void initTimer(void){
  set_bit(TCCR0A, WGM01);       /* CTC mode */
  set_bit(TCCR0A, COM0A0);      /* Toggles pin each cycle through */
  set_bit(TCCR0B, CS00);
  set_bit(TCCR0B, CS01);        /* CPU clock / 64 */
}

static inline void playNote(uint8_t wavelength, uint16_t duration){
  OCR0A = wavelength;           /* set pitch */
  set_bit(SPEAKER_DDR, SPEAKER);/* enable output on speaker */

  while(duration){              /* Variable delay */
    _delay_ms(1);
    duration--;
  }
  clear_bit(SPEAKER_DDR, SPEAKER); /* turn speaker off */
}

int main(void){

  // -------- Inits --------- //
 
  initTimer();
  initUART();
  printString("hello");
  while(1){
    
    playNote(C2,50);
    _delay_ms(1000);

  }   
  // ------ Event loop ------ //
  return(0);                  /* This line is never reached  */
}

