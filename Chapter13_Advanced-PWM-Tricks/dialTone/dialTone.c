/* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include <avr/interrupt.h>	
#include <avr/power.h>
#include "pinDefines.h"
#include "macros.h"
#include "fullSine.h"


static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
}

static inline void pollButton(void){
  if (bit_is_clear(BUTTON_PIN, BUTTON)) {
    set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */
    clear_bit(LED_PORT, LED0);
  }
  else {
    clear_bit(SPEAKER_DDR, SPEAKER); /* disable output on pin */
    set_bit(LED_PORT, LED0);
  }
}

static inline void initLED(void){
  set_bit(LED_DDR, LED0);	/* LED on for diagnostics */
  set_bit(LED_PORT, LED0);
  _delay_ms(100);
  clear_bit(LED_PORT, LED0);
}

int main(void){

  volatile  uint16_t accumulator0;  
  volatile  uint16_t accumulator1;  
  volatile  uint16_t tuningWord0;
  volatile  uint16_t tuningWord1;
  volatile  uint16_t mixer;

  // -------- Inits --------- //
  
  clock_prescale_set(clock_div_1);                  /* CPU clock 8 MHz */
  initLED();
  initTimer0();
  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  tuningWord0 = 440*2;		/* Dial tone frequencies */
  tuningWord1 = 350*2;
  
  // ------ Event loop ------ //
  while(1){		       
    
    loop_until_bit_is_set(TIFR0, TOV0); /* wait until overflow bit set */
    set_bit(TIFR0, TOV0);		/* writing set should reset... */

    //  Note that this bit has to be pretty fast... we have only 256 cycles to
    //  set the next value in OCR0A, or we may hear a glitch

    accumulator0 += tuningWord0;	/* take tuningWord steps forward */
    accumulator1 += tuningWord1;	/* take tuningWord steps forward */

    mixer = fullSine[(uint8_t) (accumulator0 >> 8)]; /* add together */
    mixer += fullSine[(uint8_t) (accumulator1 >> 8)];
    OCR0A = 128 + (mixer >> 1);		/* divide by 2, the fast way */
    
    pollButton();
    

  } /* End event loop */
  return 0;		      /* This line is never reached  */
}



