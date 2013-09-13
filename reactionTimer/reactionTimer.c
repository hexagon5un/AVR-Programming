/* 
   Press the button as quickly as you can after the LEDs light up.
   Your time is printed out over the serial port.  
*/

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>         
#include "pinDefines.h"
#include "USART.h"

#include "support.h"

static inline void initTimer1(void){
  /* Normal mode (default), just counting */
  TCCR1B |= (1<<CS11) | (1<<CS10);
  /* Clock speed: 1 MHz / 64, 
     each tick is 64 microseconds ~= 15.6 per ms  */	
  /* No special output modes */
}

int main(void){
  char byte;
  uint16_t timerValue;

  // -------- Inits --------- //
	
	initUSART();
  initTimer1();
  LED_DDR = 0xff;		/* all LEDs for output */
  BUTTON_PORT |= (1<<BUTTON);	/* enable internal pull-up */

  printString("\r\nReaction Timer:\r\n");
  printString("---------------\r\n");
  printString("Press any key to start.\r\n");

  // ------ Event loop ------ //
  while(1){			

    byte = receiveByte();	/* press any key */
    printString("\r\nGet ready...");
    randomDelay();

    printString("\r\nGo!\r\n");
    LED_PORT = 0xff;		/* light LEDs */
    TCNT1 = 0;			/* reset counter */

    if (bit_is_clear(BUTTON_IN, BUTTON)){ 
      /* Button pressed _exactly_ as LEDs light up.  Suspicious. */
      printString("You're only cheating yourself.\r\n");
    }
    else{
      // Wait until button pressed, save timer value.
      loop_until_bit_is_clear(BUTTON_IN, BUTTON);
      timerValue = TCNT1 >> 4; 
      /* each tick is approx 1/16 milliseconds, so we bit-shift divide */
      
      printMilliseconds(timerValue);	
      printComments(timerValue);
    }
    
    // Clear LEDs and start again.
    LED_PORT = 0x00;
    printString("Press any key to try again.\r\n");
     
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}
