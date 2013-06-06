/* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include <avr/interrupt.h>	
#include "pinDefines.h"
#include "macros.h"
#include "fullSine.h"
#include "USART.h"

volatile  uint16_t accumulator;  
volatile  uint16_t tuningWord;

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);   /* Clock with /1 prescaler */
  set_bit(TIMSK0, TOIE0);  /* Overflow interrupt */
  sei();		   /* Enable interrupt */
}

ISR(TIMER0_OVF_vect){

  accumulator += tuningWord;	/* take tuningWord steps forward */
  OCR0A = 128 + fullSine[(uint8_t) (accumulator >> 8)]; /* lookup and set output */

}

static inline void pollButton(void){
  if (bit_is_clear(BUTTON_IN, BUTTON)) {
    set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */
    clear_bit(LED_PORT, LED0);
  }
  else {
    clear_bit(SPEAKER_DDR, SPEAKER); /* disable output on pin */
    set_bit(LED_PORT, LED0);
  }
}

int main(void){

  // -------- Inits --------- //

  initTimer0();
  initUART();
   
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */

  set_bit(LED_DDR, LED0);	/* LED on for diagnostics */
  set_bit(LED_PORT, LED0);
  _delay_ms(100);
  clear_bit(LED_PORT, LED0);
   
  set_bit(SPEAKER_DDR, SPEAKER);
  
  tuningWord = 880;
  transmitByte((uint8_t) tuningWord >> 2); /* approx value in Hz */

  // ------ Event loop ------ //
  while(1){		       

    pollButton();
        
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



