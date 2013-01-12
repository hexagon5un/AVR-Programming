/*  Demo using pin-change interrupts and in-ISR debouncing routine  */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "macros.h"

ISR(PCINT2_vect){                 /* Run every time button state changes */
  if (bit_is_set(BUTTON_IN, BUTTON)){
    set_bit(LED_PORT, LED1);
  }
  else{
    clear_bit(LED_PORT, LED1);
  }
}

void initPinChangeInterrupt18(void){
  set_bit(PCICR, PCIE2);     /* set pin-change interrupt for D pins */
  set_bit(PCMSK2, PCINT18);  /* set mask to look for PCINT18 / PD2 */
  sei();		     /* set (global) interrupt enable bit */
}

int main(void){
  // -------- Inits --------- //
  LED_DDR = 0xff;               /* all LEDs active */
  set_bit(BUTTON_PORT, BUTTON); /* pullup */
  initPinChangeInterrupt18();
  
  // ------ Event loop ------ //
  while(1){     

    _delay_ms(200);
    toggle_bit(LED_PORT, LED0);
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}
