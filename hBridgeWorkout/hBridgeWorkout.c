/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"

static inline void setBridgeState(uint8_t bridgeA, uint8_t bridgeB){
  if (bridgeA){
    set_bit(PORTD, PD6);	
    set_bit(LED_PORT, LED0);  
  }
  else {
    clear_bit(PORTD, PD6);	
    clear_bit(LED_PORT, LED0);  
  }

  if (bridgeB){
    set_bit(PORTD, PD5);	
    set_bit(LED_PORT, LED1);  
  }
  else {
    clear_bit(PORTD, PD5);	
    clear_bit(LED_PORT, LED1);  
  }
}


int main(void){
  // -------- Inits --------- //

  set_bit(DDRD, PD6);  /* now hooked up to bridge, input1 */
  set_bit(DDRD, PD5);  /* now hooked up to bridge, input2 */
  set_bit(LED_DDR, LED0);  
  set_bit(LED_DDR, LED1);  

  // ------ Event loop ------ //
  while(1){     
    
    setBridgeState(1,0);	/* "forward" */
    _delay_ms(2000);
    
    setBridgeState(0,0);	/* both low stops motor */
    _delay_ms(2000);
    
    setBridgeState(0,1);	/* "reverse" */
    _delay_ms(2000);
    
    setBridgeState(1,1);	/* both high also stops motor */
    _delay_ms(2000);

    // For extra-quick braking, energize the motor backwards
    setBridgeState(1,0);
    _delay_ms(2000);
    setBridgeState(0,1);
    _delay_ms(75);	     /* tune this time to match your system */
    setBridgeState(0,0);
    _delay_ms(2000);	     
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

