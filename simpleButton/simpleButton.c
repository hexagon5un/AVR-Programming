/* 
   Demo of the simplest on/off button code
*/

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>

#define BUTTON         PD2
#define BUTTON_PORT    PORTD
#define BUTTON_DDR     DDRD
#define BUTTON_PINS    PIND

#define LED_PORT PORTB
#define LED_DDR  DDRB

int main(void){
  // -------- Inits --------- //
  BUTTON_PORT = (1 << BUTTON);/* initialize pullup resistor on our input pin */
  LED_DDR = 0xff;	      /* set up all LEDs for output */

  // ------ Event loop ------ //
  while(1){                       
    if (bit_is_clear(BUTTON_PINS, BUTTON)){  /* look for button press */
      /* equivalent to if ( BUTTON_PINS & (1<<BUTTON) ){ */
      LED_PORT = 0b10101010;			  
    }  
    else{
      LED_PORT = 0b01010101;                 
    }
  } /* End event loop */
  return(0);
}
