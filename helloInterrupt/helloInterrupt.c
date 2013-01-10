/* 

Demo of using interrupts for doing what they do best -- 
two things at once.

Flashes LED0 at a fixed rate, interrupting whenever button is pressed.

 */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "macros.h"

ISR(INT0_vect){                 /* Run every time there is a change on button */
  if (bit_is_set(BUTTON_IN, BUTTON)){
    set_bit(LED_PORT, LED1);
  }
  else{
    clear_bit(LED_PORT, LED1);
  }
}

void initInterrupt0(void){
  set_bit(EIMSK, INT0);        /* enable INT0 */
  set_bit(EICRA, ISC01);       /* trigger when button changes */
  sei();                       /* set (global) interrupt enable bit */
}

int main(void){
  // -------- Inits --------- //
  LED_DDR = 0xff;               /* all LEDs active */
  set_bit(BUTTON_PORT, BUTTON); /* pullup */
  initInterrupt0();
  
  // ------ Event loop ------ //
  while(1){     
    
    _delay_ms(200);
    toggle_bit(LED_PORT, LED0);

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}
