/* 
loopback.c

A simple test of serial-port functionality.  
Takes in a character at a time and sends it right back out,
 blinking the LED along the way.
*/

// ------- Preamble -------- //
#include <avr/io.h>		
#define F_CPU 1000000UL	       
#include <util/delay.h>		
#define BAUDRATE  9600
#include "USART.h"

#define LED       PB0
#define LED_PORT  PORTB
#define LED_DDR   DDRB

inline void initLED(void){
  // Set output mode
  LED_DDR |= (1 << LED);
  
  uint8_t i = 3;
  while(i){  // Blink a few times to make sure all working 
    LED_PORT |= (1 << LED);
    _delay_ms(20);
    LED_PORT &= ~(1 << LED);
    _delay_ms(100);
    i--;
  }
}

int main(void){
  char serialCharacter;

  // -------- Inits --------- //
  initLED();
  initUSART();
  sayOK();

  // ------ Event loop ------ //
  while(1){	

    serialCharacter = receiveByte(); 
    transmitByte(serialCharacter);
    LED_PORT ^= (1 << LED);	/* toggle LED state on receive and echo */
    
  }   /* End event loop */
  return(0);
}





