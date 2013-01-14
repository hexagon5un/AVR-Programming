/*  */

// ------- Preamble -------- //
#include "reactionTimer.h"

static inline void initTimer1(void){
  /* Normal mode (default) */
  set_bit(TCCR1B, CS10);
  set_bit(TCCR1B, CS12);	/* Clock speed: 8 MHz / 1024 */
			/* each tick is approx 1/8 milliseconds */
}

int main(void){
  char byte;
  uint16_t timerValue;

  // -------- Inits --------- //
  initUSART();
  initTimer1();
  LED_DDR = 0xff;		/* all LEDs for output */
  set_bit(BUTTON_PORT, BUTTON);	/* pull-up on button */

  transmitString("\r\nReaction Timer:\r\n");
  transmitString("---------------\r\n");
  transmitString("Press any key to start.\r\n");

  // ------ Event loop ------ //
  while(1){			

    byte = receiveByte();	/* wait for input */
    transmitString("\r\nGet ready...");
    randomDelay();

    transmitString("\r\nGo!\r\n");
    LED_PORT = 0xff;		/* light LEDs */
    TCNT1 = 0;			/* reset counter */

    if (bit_is_clear(BUTTON_IN, BUTTON)){
      transmitString("Cheater...\r\n");
      /* Holding the button down before the LEDs light up is cheating. */
    }
    else{
      // Wait until button pressed, save timer value.
      loop_until_bit_is_clear(BUTTON_IN, BUTTON);
      timerValue = TCNT1 >> 3; 
      /* each tick is approx 1/8 milliseconds, so we bit-shift divide */
      
      printMilliseconds(timerValue);	
      printComments(timerValue);
    }
    
    // Clear LEDs and start again.
    LED_PORT = 0x00;
    transmitString("Press any key to try again.\r\n");
     
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}
