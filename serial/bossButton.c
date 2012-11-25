/* 
bossButton.c

As long as we've got a button wired up to the AVR, 
might as well have some fun.

Upload this code to your AVR, run bossButton.py.
Press button.

*/

// ------- Preamble -------- //
#include <avr/io.h>		
#include <util/delay.h>		
#define BAUDRATE  9600
#include "USART.h"

#define LED       PB0
#define LED_PORT  PORTB
#define LED_DDR   DDRB
#define BUTTON                  PD2
#define BUTTON_PORT             PORTD
#define BUTTON_PIN              PIND
#define BUTTON_DDR              DDRD

static inline void blinkLED(void){
  LED_PORT = (1 << LED);
  _delay_ms(200);
  _delay_ms(200);
  _delay_ms(200);
  _delay_ms(200);
  LED_PORT &= ~(1 << LED);
}


int main(void){
  char serialCharacter;

  // -------- Inits --------- //
  LED_DDR = (1 << LED);
  blinkLED();

  initUSART();
  transmitByte('O');

  BUTTON_PORT |= (1 << BUTTON);	/* input mode, turn on pullup */
  
  // ------ Event loop ------ //
  while(1){	

    if (bit_is_clear(BUTTON_PIN, BUTTON)){
      transmitByte('X');
      blinkLED();
    }
    
  }   /* End event loop */
  return(0);
}





