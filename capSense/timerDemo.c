/*  */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //
volatile uint8_t ticks=0;	/* 196 cycles ~= 25 milliseconds */
volatile uint8_t seconds=0;	/* 40 ticks = 1 second */
volatile uint8_t minutes=0;	/* 60 sec = 1 minute */

// -------- Functions --------- //
ISR(TIMER0_COMPA_vect){
  ticks++;
  toggle_bit(LED_PORT, LED0);

  if (ticks == 40){		/* roughly every second */
    ticks = 0;
    seconds++;
    toggle_bit(LED_PORT, LED1);
  }

  if (seconds == 60){
    seconds = 0;
    minutes++;
    toggle_bit(LED_PORT, LED2);  
  }
}

static inline void initTimerTicks(void){
  set_bit(TCCR0A, WGM01);	/* CTC mode */
  set_bit(TCCR0B, CS00);
  set_bit(TCCR0B, CS02);	/* 8 MHz / 1024: 7812.5 Hz */
  set_bit(TIMSK0, OCIE0A); 	/* output compare interrupt enable*/
  OCR0A = 195;			/* 8 Mhz / 1024 / 196 ~= 25 ms */
  sei();			/* set enable interrupt bit */
}

int main(void){
  
  // -------- Inits --------- //
  initTimerTicks();
  set_bit(LED_DDR, LED0); 
  set_bit(LED_DDR, LED1); 
  set_bit(LED_DDR, LED2); 

  // ------ Event loop ------ //
  while(1){	

    ; 				/* empty event loop! */

  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}
