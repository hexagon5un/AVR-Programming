/* 
   Demo of the 

*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  1000000UL
#include <util/delay.h>


#define LED_DDR       DDRB	/* all LEDs hooked up to one port */
#define LED_PORT      PORTB

#define BUTTON_DDR    DDRD	/* button on PD4 */
#define BUTTON_INPUT  PIND
#define BUTTON_PORT   PORTD
#define BUTTON        PD4

#define DEBOUNCE_TIME  10

void waitForButton(void){
  loop_until_bit_is_set(BUTTON_INPUT, BUTTON);   /* make sure start out un-pressed */
  _delay_ms(DEBOUNCE_TIME);
  loop_until_bit_is_set(BUTTON_INPUT, BUTTON);   /* make sure start out un-pressed */
  
  loop_until_bit_is_clear(BUTTON_INPUT, BUTTON); /* wait for first press */
  _delay_ms(DEBOUNCE_TIME);
  loop_until_bit_is_clear(BUTTON_INPUT, BUTTON);   
  
}

void blinkTest(uint8_t times){
  do{			       
    LED_PORT = 0xff;
    _delay_ms(100);
    LED_PORT = 0;
    _delay_ms(200);  
    times--;
  } while (times);
}

void init(void){
  // Init LED port for all output
  LED_DDR = 0xff;             

  // Init button, input with pullup resistor
  BUTTON_DDR &= ~(1<< BUTTON);   /* not necessary, but double-sure in input mode */
  BUTTON_PORT |= (1 << BUTTON);  /* activate pullup */
}


int main(void){
  
  init();
  blinkTest(3);

  while(1){			/* mainloop */

    waitForButton();
    LED_PORT = (1 << 0);

    waitForButton();
    LED_PORT = (1 << 2);

    waitForButton();
    LED_PORT = (1 << 4);

    waitForButton();
    LED_PORT = (1 << 6);
    
  } /* end mainloop */
  return(0);
}
