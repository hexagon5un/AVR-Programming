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

#define LED_PORT  PORTB
#define LED_DDR   DDRB

inline void initLED(void){
  // Set output mode for whole B bank
  LED_DDR = 0xff;
  
  // Blink LEDs to make sure all working 
  uint8_t i = 0;
  for(i=0; i<8; i++){  
    LED_PORT = (1 << i);
    _delay_ms(100);
  }
  LED_PORT = 0;
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
    LED_PORT = serialCharacter;	  /* ascii/numeric value of character */
    
  }   /* End event loop */
  return(0);
}





