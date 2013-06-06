/*  */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "UART.h"

#define FUDGE_FACTOR 28

// -------- Global Variables --------- //
volatile uint8_t ticks=0;	/*  */
volatile uint8_t seconds=0;	/* 125 ticks per second  */
volatile uint8_t minutes=0;	/* 60 sec = 1 minute */
volatile uint8_t ledState=0;	/* start all LEDs off */

// -------- Functions --------- //
ISR(TIMER0_COMPA_vect){
  ticks++;			
  
  if (ticks == 10){
    TCNT0 += FUDGE_FACTOR;
  }
  
  if (ticks == 130){		/* roughly every second */
    ticks = 0;
    seconds++;
    toggle_bit(LED_PORT, LED0);
  }

  if (seconds == 60){		/* roughly every minute */
    seconds = 0;
    minutes++;
  }

  

}

static inline void initTimerTicks(void){
  set_bit(TCCR0A, WGM01);	/* CTC mode */
  set_bit(TCCR0B, CS02);	/* 8 MHz / 256: 31,250 Hz */
  set_bit(TIMSK0, OCIE0A); 	/* output compare interrupt enable*/
  OCR0A = 250;			/* 31,250 Hz / 250 = 125 Hz */
  sei();			/* set enable interrupt bit */
}

int main(void){
  
  // -------- Inits --------- //
  initTimerTicks();
  LED_DDR = (1 << LED0) | (1 << LED1) | (1 << LED2);

  // ------ Event loop ------ //
  while(1){	

    
  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}
