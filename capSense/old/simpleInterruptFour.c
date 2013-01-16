/*  */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"


// -------- Global Variables --------- //
volatile uint8_t pressed;	

// -------- Functions --------- //

ISR(INT0_vect){ 		/* Run when button pressed */
    pressed = 1;
}

void initInterrupt0(void){
  set_bit(EIMSK, INT0);	       /* enable INT0 */
  set_bit(EICRA, ISC01);       /* trigger on falling edge */
  sei();		       /* set (global) interrupt enable bit */
}

int main(void){

  uint8_t ledMask = 2;

  // -------- Inits --------- //
  LED_DDR = 0xff;  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup */
  initInterrupt0();

  // ------ Event loop ------ //
  while(1){	

    /* State change in Interrupt, debounce in mainloop */
    /* Advantage is state-change taken care of by int */
    /* Delay is in mainloop, so pre-emptable by interrupts, 
     so can use a simple _delay_ms() */
    /* OTOH, if mainloop is slow, response is slow */
    if (pressed){
      ledMask = ledMask << 1;
      if (!ledMask){
	ledMask = 2;
      }
      LED_PORT = ledMask;
      pressed = 0;
    }
    
    /* Simulate something to do */
    _delay_ms(500);
    toggle_bit(LED_PORT, LED0);

  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}


