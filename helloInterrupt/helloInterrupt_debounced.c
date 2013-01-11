/*  Demo using pin-change interrupts and in-ISR debouncing routine  */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "macros.h"

#define DEBOUNCE_TIME 2         /* milliseconds */

ISR(INT0_vect){                 /* Run every time button state changes */
  _delay_ms(DEBOUNCE_TIME);
  if (bit_is_clear(BUTTON_IN, BUTTON)){
    LED_PORT = (LED_PORT << 1) ; /* roll to next LED */
    if (!LED_PORT){		 /* if rolled off the left end... */
      LED_PORT = (1 << LED1);	 /* start back at LED1 */
    }
  }
}

void initPinChangeInterrupt18(void){
  set_bit(PCICR, PCIE2);	/* set pin-change interrupt for D pins */
  set_bit(PCMSK2, PCINT18);	/* set mask to look for PCINT18 / PD2 */
  sei();                       /* set (global) interrupt enable bit */
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
