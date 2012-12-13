/*  

Quick and dirty PWM Demo

*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

static inline void initTimer2(void){
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


int main(void){
  
  uint8_t brightness;

  // -------- Inits --------- //

  initTimer2();
  OCR2A = 0;

  initUSART();
  _delay_ms(1000);
  transmitString("Type a number 0-9\r\n");
  
  // ------ Event loop ------ //
  while(1){	
    
    brightness = receiveByte();
    if ((brightness >= '0') && (brightness <= '9')){
      OCR2A = (brightness - '0') * 28;
      echo(brightness);
    }
    

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}




