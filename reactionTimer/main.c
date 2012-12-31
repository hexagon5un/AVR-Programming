/*  */

// ------- Preamble -------- //
#include "main.h"

int main(void){
  char byte;
  uint16_t timerValue;
  uint8_t randomTime;

  // -------- Inits --------- //
  initUSART();
  transmitString("\r\nReaction Timer:\r\n");
  transmitString("---------------\r\n");
  transmitString("Press any key to start.\r\n");
  initTimer1();
  LED_DDR = 0xff;
  set_bit(BUTTON_PORT, BUTTON);

  // ------ Event loop ------ //
  while(1){			

    byte = receiveByte();
    transmitString("\r\nGet ready...");
    _delay_ms(1000);		  /* wait at least one second */

    // "random" extra time up to 2.5 sec 
    randomTime = (uint8_t) TCNT1; /* take least-significant byte from timer */
    while(--randomTime){  /* not really random, but hard to control */
      _delay_ms(10);		/* (like coin-flipping) */
    }

    transmitString("\r\nGo!\r\n");
    LED_PORT = 0xff;
    TCNT1 = 0;			/* reset counter */

    if (bit_is_clear(BUTTON_IN, BUTTON)){
      transmitString("Cheater...\r\n");
      /* If you pressed within 1/8 microsecond, you're cheating. */
    }
    else{
      // Wait until button pressed, report value.
      loop_until_bit_is_clear(BUTTON_IN, BUTTON);
      timerValue = TCNT1 >> 3; /* each tick is approx 1/8 milliseconds */
      
      transmitString("\r\nYou took ");
      print16(timerValue);	
      transmitString(" seconds.\r\n");
      printComments(timerValue);
    }
    
    // Clear LEDs and start again.
    LED_PORT = 0x00;
    transmitString("Press any key to try again.\r\n");
     
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}

static inline void print16(uint16_t value){
  uint8_t digit;
  digit = 0;
  while(value >= 10000){
    digit++;
    value -= 10000;
  }
  if (digit){
    transmitByte('0'+digit);
  }
  digit = 0;
  while(value >= 1000){
    digit++;
    value -= 1000;
  }
  transmitByte('0'+digit);
  transmitByte('.');
  digit = 0;
  while(value >= 100){
    digit++;
    value -= 100;
  }
  transmitByte('0'+digit);
  digit = 0;
  while(value >= 10){
    digit++;
    value -= 10;
  }
  transmitByte('0'+digit);
  // Ones digit is easy.
  transmitByte('0'+value);
}

static inline void initTimer1(void){
  /* Normal mode (default) */
  set_bit(TCCR1B, CS10);
  set_bit(TCCR1B, CS12);	/* Clock speed: 8 MHz / 1024 */
			/* each tick is approx 1/8 milliseconds */
  /* No PWM output */
  /* No interrupts */
}

static inline void printComments(uint16_t value){
  if (value > 1000){
    transmitString("---->  Hello?\r\n");
  }
  else if (value > 500){
    transmitString("---->  Slow.\r\n");    
  }
  else if (value > 250){
    transmitString("---->  Have another cup of coffee.\r\n");    
  }
  else if (value > 200){
    transmitString("---->  Respectable.\r\n");    
  }
  else if (value >= 150){
    transmitString("---->  Fast.\r\n");    
  }
  else if (value < 150){
    transmitString("---->  Amazing!\r\n");    
  }
}
