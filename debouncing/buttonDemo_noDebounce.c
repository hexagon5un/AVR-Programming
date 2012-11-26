/* 
  Button demo with no debouncing,
  depending on the quality of your button, 
  may skip steps, or even change when you let go of button.
  
  But it'll only do it sometimes, which will drive you crazy.  
*/

#include <avr/io.h>
#include <util/delay.h>

#define BUTTON                  PD2
#define BUTTON_PORT             PORTD
#define BUTTON_PIN              PIND
#define BUTTON_DDR              DDRD

int main(void){

  uint8_t whichPin=0;
  uint8_t buttonWasPressed;

  DDRB = 0xff;                  /* all LED for output */
  BUTTON_PORT |= (1 << BUTTON);	/* pullup on button */

  while(1){			/* mainloop */

    if (bit_is_clear(BUTTON_PIN, BUTTON)){ 	/* button pressed */
      PORTB |= (1<<7);		   /* light up PB7 while pressed */ 
      if (!buttonWasPressed){	   /* if first time it's pressed */
	whichPin++;		   /* go to next pattern */
      }
      buttonWasPressed = 1;
    }
    else{			/* button not pressed */
      buttonWasPressed = 0;
      PORTB &= ~(1<<7);
    }
    
    /* limit pins to first 6 */
    if (whichPin > 5){
      whichPin = 0;
    }

    PORTB = (1 << whichPin);	/* light up the selected pin */

  } /* end mainloop */
  return(0);
}
