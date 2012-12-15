/*  

PWM Demo with serial control over three LEDs

*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

static inline void initTimers(void){
  // Timer 1 A,B
  set_bit(TCCR1A, WGM10);     /* Fast PWM mode, 8-bit */
  set_bit(TCCR1B, WGM12);     /* Fast PWM mode, pt.2 */

  set_bit(TCCR1B, CS11);      /* PWM Freq = F_CPU/8/256 */

  set_bit(TCCR1A, COM1A1);    /* PWM output on OCR1A */
  set_bit(TCCR1A, COM1B1);    /* PWM output on OCR1B */

  // Timer 2
  set_bit(TCCR2A, WGM20);     /* Fast PWM mode */
  set_bit(TCCR2A, WGM21);     /* Fast PWM mode, pt.2 */

  set_bit(TCCR2B, CS21);      /* PWM Freq = F_CPU/8/256 */

  set_bit(TCCR2A, COM2A1);    /* PWM output on OCR2A */
  
}

static inline uint8_t getBrightness(void){
  // Gets a PWM value from the serial port.
  // Reads in characters, turns them into a number
  char hundreds='0'; 
  char tens='0';
  char ones = '0';
  char thisChar = '0';

  do{
    hundreds = tens;		/* shift numbers over */
    tens = ones;
    ones = thisChar;

    thisChar = receiveByte();	/* get a new character */
    transmitByte(thisChar);	/* echo */
  } while(thisChar != '\r');

  transmitByte('\n');		/* newline */
  return(100*(hundreds-'0') + 10*(tens-'0') + ones-'0');
}

int main(void){
  
  uint8_t brightness;

  // -------- Inits --------- //

  initTimers();
  initUSART();

  /* enable output on LED pins, triggered by PWM hardware */
  set_bit(LED_DDR, LED1);       
  set_bit(LED_DDR, LED2);        
  set_bit(LED_DDR, LED3);       

  // ------ Event loop ------ //
  while(1){	    

    brightness = getBrightness();
    OCR2A = OCR1B;
    OCR1B = OCR1A;
    OCR1A = brightness;

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}




