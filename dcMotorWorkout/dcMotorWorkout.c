/* Demos PWM control of a DC motor */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define SPEED_STEP_DELAY 2	/* milliseconds */

// -------- Functions --------- //
static inline void initTimer0(void){
  set_bit(TCCR0A, WGM00);       /* Fast PWM mode */
  set_bit(TCCR0A, WGM01);       /* Fast PWM mode, pt.2 */  
  set_bit(TCCR0A, COM0B1);      /* output PWM to pin */
  set_bit(TCCR0B, CS01);        /* Clock with /1024 prescaler */
  //set_bit(TCCR0B, CS00);        /* Clock with /1024 prescaler, pt.2 */
}

static inline uint8_t getNumber(void){
  // Gets a PWM value from the serial port.
  // Reads in characters, turns them into a number
  char hundreds='0'; 
  char tens='0';
  char ones = '0';
  char thisChar = '0';

  do{
    hundreds = tens;            /* shift numbers over */
    tens = ones;
    ones = thisChar;
    thisChar = receiveByte();   /* get a new character */
    transmitByte(thisChar);     /* echo */
  } while(thisChar != '\r');

  transmitByte('\n');           /* newline */
  return(100*(hundreds-'0') + 10*(tens-'0') + ones-'0');
}

int main(void){

  uint8_t updateSpeed;

  // -------- Inits --------- //
  initTimer0();
  OCR0B = 0;

  set_bit(ANTENNA_DDR, ANTENNA);  /* now hooked up to MOSFET, output */
  set_bit(LED_DDR, LED0);  
  set_bit(LED_DDR, LED1);  
  
  initUSART();
  sayOK();
  
  // ------ Event loop ------ //
  while(1){     
    
    updateSpeed = getNumber();

    /* Ramp up/down to desired speed */
    if (OCR0B < updateSpeed){
      set_bit(LED_PORT, LED0);
      while(OCR0B < updateSpeed){
	OCR0B++;
	_delay_ms(SPEED_STEP_DELAY);
      }
    }
    else{
      set_bit(LED_PORT, LED1);
      while(OCR0B > updateSpeed){
	OCR0B--;
	_delay_ms(SPEED_STEP_DELAY);
      }
    }
    LED_PORT=0;			/* all off */

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

