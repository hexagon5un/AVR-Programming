#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>         
#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

ISR(PCINT1_vect){
  toggle_bit(LED_PORT, LED0);
}

static inline void initPinChangeInterrupt(void){
  set_bit(PCICR, PCIE1); /* enable Pin-change interrupts 1 (bank C) */
  set_bit(PCMSK1, PCINT9); /* enable specific interrupt for our pin PC1 */
  sei();		       /* set (global) interrupt enable bit */
}


int main(void){
  LED_DDR = 0xff;
  initPinChangeInterrupt();
  set_bit(CAP_SENSOR_DDR, CAP_SENSOR); /* output */
  // ------ Event loop ------ //
  while(1){	
		
    _delay_ms(100);
    toggle_bit(CAP_SENSOR_PORT, CAP_SENSOR);

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}
