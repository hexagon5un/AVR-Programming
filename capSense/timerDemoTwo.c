/*  */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define DEBOUNCE_TIME  10	/* milliseconds */

// -------- Global Variables --------- //
volatile uint8_t milliseconds = 0;	

// -------- Functions --------- //
ISR(TIMER0_COMPA_vect){
  milliseconds++;
}

static inline void initTimerTicks(void){
  set_bit(TCCR0A, WGM01);	/* CTC mode */
  set_bit(TCCR0B, CS02);	/* 8 MHz / 256 */
  set_bit(TIMSK0, OCIE0A); 	/* output compare interrupt enable*/
  OCR0A = 31;			/* 8 Mhz / 256 / 31 = 0.992 ms */
  sei();			/* set enable interrupt bit */
}

int main(void){
  uint8_t ledTime0;
  uint8_t ledTime1;

  uint8_t debouncing; 
  uint8_t checkButtonTime;
  uint8_t buttonState;
  uint8_t lastButtonState;

  // -------- Inits --------- //
  initTimerTicks();
  set_bit(LED_DDR, LED0);  
  set_bit(LED_DDR, LED1);  
  set_bit(LED_DDR, LED7); 
  set_bit(BUTTON_PORT, BUTTON);	/* pullup */
  
  // ------ Event loop ------ //
  while(1){	

    /* Two LEDs flashing on un-related schedules */
    if (milliseconds == ledTime0){		
      toggle_bit(LED_PORT, LED0);
      ledTime0 = milliseconds + 100; /* toggle every 100 ms */
    }
    if (milliseconds == ledTime1){		
      toggle_bit(LED_PORT, LED1);
      ledTime1 = milliseconds + 103; /* toggle every 103 ms */
    }

    /* Test button, with debounce, toggle LED */
    if (bit_is_clear(BUTTON_IN, BUTTON)){
      if (!buttonState){	/* isn't pressed yet*/
	if (debouncing){	/* if debouncing, see if it's time */
	  if ((milliseconds == checkButtonTime) &&
	      bit_is_clear(BUTTON_IN, BUTTON)){
	    buttonState = 1;
	    debouncing = 0;
	  }
	}
	else{	/* start debouncing if not already */
	  checkButtonTime = milliseconds + DEBOUNCE_TIME;
	  debouncing = 1;
	}
      }
    }
    else{			/* button not pressed */
      debouncing = 0;
      buttonState = 0;
    }

    /* If button state changed, toggle LED */
    if (buttonState && !lastButtonState){
      toggle_bit(LED_PORT, LED7);
    }
    lastButtonState = buttonState; /* update state */

  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}


