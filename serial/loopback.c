/* 

loopback.c

A simple test of serial-port functionality.  
Takes in a character at a time and sends it right back out.

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

#define FLASH_DELAY  20

void flashLED(){
     LED_PORT |= (1 << LED);
    _delay_ms(FLASH_DELAY);
    LED_PORT &= ~(1 << LED);
}

void initLED(){
  // Set output mode
  LED_DDR |= (1 << LED);
  
  // Blink a few times to make sure all working (optional)
  uint8_t i = 3;
  while(i){
    flashLED();
    _delay_ms(100);
    i--;
  }
}

int main(){
  char serialCharacter;

  // -------- Inits --------- //
  initLED();
  initUART();
  sayOK();

  // ------ Event loop ------ //
  while(1){	

    serialCharacter = receiveByte(); 
    transmitByte(serialCharacter);
    flashLED();
    
  }   /* End event loop */
  return(0);
}





