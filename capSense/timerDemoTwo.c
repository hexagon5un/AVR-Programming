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
volatile uint8_t debouncing;
volatile uint8_t debounceTime;


// -------- Functions --------- //
ISR(TIMER0_COMPA_vect){
  milliseconds++;
}

ISR(INT0_vect){ 		/* Run when button pressed */
  debounceTime = milliseconds + DEBOUNCE_TIME;
  debouncing = 1;
}

static inline void initTimerTicks(void){
  set_bit(TCCR0A, WGM01);	/* CTC mode */
  set_bit(TCCR0B, CS02);	/* 8 MHz / 256 */
  set_bit(TIMSK0, OCIE0A); 	/* output compare interrupt enable*/
  OCR0A = 31;			/* 8 Mhz / 256 / 31 = 0.992 ms */
  sei();			/* set enable interrupt bit */
}


static inline void initInterrupt0(void){
  set_bit(EIMSK, INT0);	       /* enable INT0 */
  set_bit(EICRA, ISC01);       /* trigger on falling edge */
  sei();		       /* set (global) interrupt enable bit */
}


int main(void){
  uint8_t ledTime0;
  uint8_t ledTime1;

  // -------- Inits --------- //
  initTimerTicks();
  initInterrupt0();
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
    
    /* If debouncing and time is up, test again and act */
    if (debouncing && (milliseconds == debounceTime)){
      if (bit_is_clear(BUTTON_IN, BUTTON)){
	toggle_bit(LED_PORT, LED7);
      }
      debouncing = 0;
    }

  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}


