/*  

Quick and dirty PWM Demo

*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

static inline void initTimers(void){
  // Timer 1 A,B
  set_bit(TCCR1A, WGM10);       /* Fast PWM mode, 8-bit */
  set_bit(TCCR1B, WGM12);       /* Fast PWM mode, pt.2 */
  set_bit(TCCR1B, CS11);        /* F_CPU/8 -- 1MHz with fast fuses */  

  set_bit(TCCR1A, COM1A1);      /* PWM output on OCR1A */
  set_bit(TCCR1A, COM1B1);      /* PWM output on OCR1B */
  set_bit(LED_DDR, LED1);       /* enable output on pin */
  set_bit(LED_DDR, LED2);       /* enable output on pin */

  // Timer 2
  set_bit(TCCR2A, WGM20);       /* Fast PWM mode */
  set_bit(TCCR2A, WGM21);       /* Fast PWM mode, pt.2 */
  set_bit(TCCR2B, CS21);        /* F_CPU/8 -- 1MHz with fast fuses */

  set_bit(TCCR2A, COM2A1);      /* PWM output on OCR2A */
  set_bit(LED_DDR, LED3);       /* enable output on pin */
}

static inline void echo(uint8_t brightness){
  transmitString("Brightness level: ");
  transmitByte(brightness);
  transmitString("\r\n");
}

static inline uint8_t generate8BitBrightness(uint8_t brightness){
  if ((brightness >= '0') && (brightness <= '9')){
      brightness = (brightness - '0') * 28;
      return(brightness);
  }
  else {
    return(0);
  }
}



int main(void){
  
  uint8_t brightness;

  // -------- Inits --------- //

  initTimers();

  initUSART();
  _delay_ms(1000);
  transmitString("Type numbers 0-9\r\n");
  
  // ------ Event loop ------ //
  while(1){	
    
    brightness = receiveByte();
    echo(brightness);
    OCR2A = OCR1B;
    OCR1B = OCR1A;
    OCR1A = generate8BitBrightness(brightness);

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}




