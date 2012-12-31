
// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define PWM_OFFSET     16 	/* values 0..32 are good */

// -------- Global Variables --------- //
volatile uint8_t ticks = 0;	

// -------- Functions --------- //
ISR(TIMER0_COMPA_vect){
  ticks++;			/* will increase to 255, loop around */
}

static inline void initTimerTicks(void){
  set_bit(TCCR0A, WGM01);	/* CTC mode */
  set_bit(TCCR0B, CS01);	/* 8 MHz / 8 clock */
  set_bit(TIMSK0, OCIE0A); 	/* output compare interrupt enable*/
  OCR0A = 50;			/* 8 Mhz / 8 /  36 =  */ 
  sei();			/* set enable interrupt bit */
}

int main(void){

  // -------- Inits --------- //
  initTimerTicks();
  LED_DDR = 0xff;  

  uint8_t i;
  int8_t ledDirection[8];
  uint8_t ledPWM[8];

  for (i=0; i<8; i++){	/* Initialize all state, direction */
    ledPWM[i] = PWM_OFFSET*i;
    ledDirection[i] = 1;
  }

  // ------ Event loop ------ //
  while(1){	
    
    if (ticks == 0){		
      LED_PORT = 0xff;		/* Turn on LEDs at beginning of timer */
      for (i=0; i<8; i++){	/* update all */
	ledPWM[i] += ledDirection[i];
	if (ledPWM[i] == 255){
	  ledDirection[i] = -1;
	}
	if (ledPWM[i] <= 1){
	  ledDirection[i] = 1;
	}
      }
    }

    for (i=0; i<8; i++){	/* Turn off if ticks == PWM value */
      if (ticks == ledPWM[i]){
	clear_bit(LED_PORT, i);
      }
    }
    

  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}

