/* 
   Demo of the simple on/off button code
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  1000000UL
#include <util/delay.h>

#define BUTTON         PD6
#define BUTTON_PORT    PORTD
#define BUTTON_DDR     DDRD
#define BUTTON_PINS    PIND     

#define LED_PORT PORTB
#define LED_DDR  DDRB

int main(void){
  
  uint8_t whichLED;

  BUTTON_PORT = (1 << BUTTON);/* initialize pullup resistor on our input pin */
  LED_DDR = 0xff;	      /* set up all LEDs for output */


  while(1){                     /* mainloop */    

    /* light up when button pressed */
    if (bit_is_clear(BUTTON_PINS, BUTTON)){       /* pin is negative logic */
      LED_PORT = (1 << whichLED);			  /* LED on */
    }  
    /* and turn off when not pressed */
    else{
      LED_PORT = 0;	                  /* all LEDs off */
    }

    // For more variety, have the LED that lights up change

    _delay_ms(100);

    whichLED++;
    if (whichLED > 7){
      whichLED = 0;
    }


  }                            /* end mainloop */
  return(0);
}
