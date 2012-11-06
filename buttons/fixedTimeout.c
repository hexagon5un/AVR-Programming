/* 
   Demo of a simple way to do button presses
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  8000000UL
#include <util/delay.h>

#define BUTTON_PIN     PD2
#define BUTTON_PORT    PORTD
#define BUTTON_DDR     DDRD
#define BUTTON_INPUT   PIND     

#define OUTPUT_PORT PORTB
#define OUTPUT_DDR  DDRB

#define DEBOUNCE_WAIT  2		/* milliseconds */

/* Global variable */
uint8_t led = 0;

static inline void incrementLED(void){
  if (led == 7){
    led = 0;			/* wrap around */
  }
  else if (led < 7){
    led++;			/* or count up */
  }
  OUTPUT_PORT = (1 << led);	/* and display */
}

void main(void){
  uint8_t buttonState;

  BUTTON_PORT = (1 << BUTTON_PIN);/* initialize pullup resistor on our input pin */
  OUTPUT_DDR = 0xff;	  /* set up LEDs for output */

  /* blink all as a sanity check */
  OUTPUT_PORT = 0xff;
  _delay_ms(100);
  OUTPUT_PORT = 0x00;
  _delay_ms(200);


  while(1){                     /* mainloop */    

    _delay_ms(5);

    if (!(BUTTON_INPUT & (1 << BUTTON_PIN)) & !buttonState){ /* pin is negative logic */
      _delay_ms(DEBOUNCE_WAIT);
      if (!(BUTTON_INPUT & (1 << BUTTON_PIN))){
	  incrementLED();
	  buttonState = 1;
      }
    }      
    
    /* Turn button state off after a delay in off state */
    if ((BUTTON_INPUT & (1 << BUTTON_PIN))){
      _delay_ms(DEBOUNCE_WAIT);
      if ((BUTTON_INPUT & (1 << BUTTON_PIN))){
	buttonState = 0;
      }
    }

  } /* end mainloop */
  
}
