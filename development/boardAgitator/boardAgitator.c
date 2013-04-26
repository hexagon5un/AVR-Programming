/* Quick interactive demo running servo with Timer 1 */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"

#define PULSE_MIN         1200	
#define PULSE_MAX         1700	
#define DELAY             20
#define STEPS             10

static inline void initTimer1Servo(void){
  /* Set up Timer1 (16bit) to give a pulse every 20ms*/
  set_bit(TCCR1A, WGM11); /* Using Fast PWM mode */
  set_bit(TCCR1B, WGM12); /* counter max in ICR1 */
  set_bit(TCCR1B, WGM13); 
 
  set_bit(TCCR1B, CS11);        /* /8 prescaling -- counting in microseconds*/
  ICR1 = 20000;                 /* TOP value = 20ms */

  set_bit(TCCR1A, COM1A1);	/* Direct output on PB1 / OC1A */
  set_bit(DDRB, PB1);		/* set pin direction to output */
}

int main(void){

  // -------- Inits --------- //

  initTimer1Servo();          
  OCR1A = PULSE_MAX;
  _delay_ms(1000);
  _delay_ms(1000);
  _delay_ms(1000);
 
  // ------ Event loop ------ //
  while(1){     

    while (OCR1A < PULSE_MAX){
      OCR1A += STEPS;
      _delay_ms(DELAY);
    }
    
    _delay_ms(500);
    
    while (OCR1A > PULSE_MIN){
      OCR1A-= STEPS;
      _delay_ms(DELAY);
    }

    _delay_ms(500);

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}
