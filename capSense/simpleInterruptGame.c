/* 

Simple game that makes use of interrupts with debounce 
LED0 toggles on and off randomly.  When the button is pressed,
it's state is copied over to the next LED.  

The game is to get all of the LEDs lit up.

*/

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define DEBOUNCE_TIME 2 	/* milliseconds */
#define LED_ON_TIME   250 	/* milliseconds,
				 set lower for harder version of game 
				 250 is doable, 200 is hard, 150 is impossible(!?)
				*/

ISR(PCINT2_vect){ 		
  _delay_ms(DEBOUNCE_TIME);
  if (bit_is_clear(BUTTON_IN, BUTTON)){
    /* Shift LEDs over, but leave the first bit on if it was already */
    LED_PORT = (LED_PORT << 1) | (LED_PORT & (1 << 0));
    TCNT0 = 0; 			/* reset timer for extra randomness */
  }
}

void initPinChangeInterrupt18(void){
  set_bit(PCICR, PCIE2);     /* set pin-change interrupt for D pins */
  set_bit(PCMSK2, PCINT18);  /* set mask to look for PCINT18 / PD2 */
  sei();                     /* set (global) interrupt enable bit */
}

void initTimer0(void){
  set_bit(TCCR0B, CS01);	/* set timer fast for "random" number */
}

int main(void){
  uint8_t i;
  
  // -------- Inits --------- //
  LED_DDR = 0xff;		/* all LEDs active */
  set_bit(BUTTON_PORT, BUTTON);	/* pullup */
  initPinChangeInterrupt18();
  initTimer0();

  // ------ Event loop ------ //
  while(1){	

    /* LED of for a random time */
    clear_bit(LED_PORT, LED0);
    _delay_ms(350);		/* minimum delay time */
    i = TCNT0;			/* "random" number */
    do{				/* extra delay */
      _delay_ms(5);
    } while(i--);
    
    /* LED on */
    set_bit(LED_PORT, LED0);
    _delay_ms(LED_ON_TIME);

    /* If managed to get all LEDs to the other side, win! */
    if (LED_PORT == 0xff){	
      for (i=0; i<20; i++){	/* Celebrate */
	LED_PORT = 0;
	_delay_ms(50);
	LED_PORT = 0xff;
	_delay_ms(50);
      }
      LED_PORT = 0;		/* And reset */
    }

  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}
