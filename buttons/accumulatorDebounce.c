/* 
   Demo of the way _not_ to do simple button presses
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  8000000UL
#include <util/delay.h>
#define BAUDRATE 9600
#include "USART88.h"

#define BUTTON_PIN     PD2
#define BUTTON_PORT    PORTD
#define BUTTON_DDR     DDRD
#define BUTTON_INPUT   PIND     

#define OUTPUT_PORT PORTB
#define OUTPUT_DDR  DDRB

#define UPPER_THRESHOLD  5
#define LOWER_THRESHOLD  1
#define COUNT_MAX  10

/* Global variable */
uint8_t led = 0;

static inline void incrementLED(void){
  if (led == 7){
    led = 0;			/* wrap around */
  }
  else if (led < 7){
    led++;			/* or count up */
  }
}

void main(void){
  uint8_t pushCount;  
  uint8_t buttonState;
  
  BUTTON_PORT = (1 << BUTTON_PIN);/* initialize pullup resistor on our input pin */
  OUTPUT_DDR = 0xff;	  /* set up LEDs for output */

  initUART();
  sayOK();

  /* blink all as a sanity check */
  OUTPUT_PORT = 0xff;
  _delay_ms(100);
  OUTPUT_PORT = 0x00;
  _delay_ms(200);


  while(1){                     /* mainloop */    

    OUTPUT_PORT = (1 << led);
    _delay_ms(5);


    if (!(BUTTON_INPUT & (1 << BUTTON_PIN))){       /* pin is negative logic */
      if (pushCount < COUNT_MAX){
	pushCount++;
      }
    }
    else {
      if (pushCount > 0){
	pushCount--;
      }
    }
    
    if ((pushCount > UPPER_THRESHOLD) & (buttonState == 0)){
      incrementLED();
      buttonState = 1;		/* signal that button is currently pushed */
    }
    if (pushCount < LOWER_THRESHOLD){
      buttonState = 0;
    }

  } /* end mainloop */
  
}
