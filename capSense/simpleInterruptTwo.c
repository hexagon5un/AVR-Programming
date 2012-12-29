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

ISR(INT0_vect){ 		/* Run every time there is a change on button */
  if(bit_is_clear(BUTTON_IN, BUTTON)){
    pressed = 1;
  }
  else{
    pressed=0;
  }
}

void initInterrupt0(void){
  set_bit(EIMSK, INT0);	       /* enable INT0 */
  set_bit(EICRA, ISC00);       /* trigger when button changes state */
  sei();		       /* set (global) interrupt enable bit */
}

int main(void){

  // -------- Inits --------- //
  set_bit(LED_DDR, LED0);  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup */
  initInterrupt0();

  // ------ Event loop ------ //
  while(1){	
    
    while(pressed){
      _delay_ms(100);
      toggle_bit(LED_PORT, LED0);
    }

  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}


