/* 
   Demo of a simple way to do button presses
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  1000000UL
#include <util/delay.h>

#define BUTTON         PD6
#define BUTTON_PORT    PORTD
#define BUTTON_PINS    PIND     

#define OUTPUT_PORT PORTB
#define OUTPUT_DDR  DDRB

#define DEBOUNCE_DELAY  5		/* milliseconds */

uint8_t incrementLED(uint8_t ledBits){
  ledBits = ledBits << 1;	/* roll to the left */

  if (!ledBits){		/* check if no bits remain */
    ledBits = 0b00000001; 		/* enable the first bit */
  }

  return(ledBits);
}

void blinkAll(void){
   // blink all as a sanity check 
  OUTPUT_PORT = 0xff;
  _delay_ms(100);
  OUTPUT_PORT = 0x00;
  _delay_ms(200);
  OUTPUT_PORT = 0xff;
  _delay_ms(100);
  OUTPUT_PORT = 0x00;
  _delay_ms(200);
}

int main(void){
  
  uint8_t buttonState;

  BUTTON_PORT = (1 << BUTTON);/* initialize pullup resistor on our input pin */
  OUTPUT_DDR = 0xff;	  /* set up LEDs for output */
  blinkAll();

  while(1){                     /* mainloop */    

    /* Debounce press */
    if (bit_is_clear(BUTTON_PINS, BUTTON)){   /* pin is negative logic */
      _delay_ms(DEBOUNCE_DELAY);
      if (bit_is_clear(BUTTON_PINS, BUTTON)){ /* still pressed */
	if (buttonState == 0){		    /* changing state from unpressed to pressed */
	  OUTPUT_PORT = incrementLED(OUTPUT_PORT);
	}
	buttonState = 1;
      }     
    }
   
    /* Debounce release */
    if (bit_is_set(BUTTON_PINS, BUTTON)){
      _delay_ms(DEBOUNCE_DELAY);
      if (bit_is_set(BUTTON_PINS, BUTTON)){ /* still released */
	buttonState = 0;
      }
    }
    

  } /* end mainloop */
  return(0);
}
