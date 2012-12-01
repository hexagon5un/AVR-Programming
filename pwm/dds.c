/* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include <avr/interrupt.h>	
#include "pinDefines.h"
#include "macros.h"
#include "fullWaves.h"
#include "USART.h"

volatile  uint16_t accumulator;  
volatile  uint16_t tuningWord;

volatile  uint8_t volume;

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
  set_bit(TIMSK0, TOIE0);  /* Overflow interrupt */
  sei();   /* Enable interrupt */
}

ISR(TIMER0_OVF_vect){
  int8_t newValue;
  accumulator += tuningWord;	/* take tuningWord steps forward */
  newValue = fullSine[(uint8_t) (accumulator >> 8)];

  // Scale by volume test
  newValue = newValue - 128;
  newValue = newValue >> volume;
  OCR0A = newValue + 128;

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

  uint16_t pitch;

  // -------- Inits --------- //

  initTimer0();
  initUSART();
  transmitByte(volume);
   
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */

  set_bit(LED_DDR, LED0);	/* LED on for diagnostics */
  set_bit(LED_PORT, LED0);
  _delay_ms(100);
  clear_bit(LED_PORT, LED0);
   
  set_bit(SPEAKER_DDR, SPEAKER);
  
  tuningWord = 400;
  // tuningWord = 1;
  volume = 0;

  // ------ Event loop ------ //
  while(1){		       

    // Wait for button press
    clear_bit(SPEAKER_DDR, SPEAKER); /* disable output on pin */
    set_bit(LED_PORT, LED0);	     /* turn on LED */
    loop_until_bit_is_clear(BUTTON_IN, BUTTON); /* wait */

    tuningWord = 3200;
    clear_bit(LED_PORT, LED0);	   /* turn off LED */
    set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

    // attack
    volume = 5; 
    _delay_ms(25);
    volume = 2; 
    _delay_ms(25);
    volume = 1; 
    _delay_ms(25);
    
    // sustain
    volume = 0; 
    _delay_ms(100);

    // decay
    while(volume < 8){
      volume++;
      tuningWord -= 100;
      _delay_ms(100);
      transmitByte(tuningWord >> 3);
    }
    
    
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



