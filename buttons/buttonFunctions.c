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

#define DEBOUNCE_TIME   5
#define ANIMATION_TIME  400

uint8_t isButtonPressed(void){
  uint8_t buttonState = 0;
  if (bit_is_clear(BUTTON_INPUT, BUTTON)){   /* first test is pressed */
    _delay_ms(DEBOUNCE_TIME);		     /* wait */
    if (bit_is_clear(BUTTON_INPUT, BUTTON)){ /* second test is pressed */
      buttonState = 1;			     /* declare it pressed */
    }
  }
  return(buttonState); 
}

uint8_t isButtonReleased(void){
  uint8_t buttonState = 0;
  if (bit_is_set(BUTTON_INPUT, BUTTON)){   /* first test is released */
    _delay_ms(DEBOUNCE_TIME);		     /* wait */
    if (bit_is_set(BUTTON_INPUT, BUTTON)){ /* second test is released */
      buttonState = 1;			     /* declare it released */
    }
  }
  return(buttonState); 
}

void blinkTest(uint8_t times){
  do{			       
    LED_PORT = 0xff;
    _delay_ms(50);
    LED_PORT = 0;
    _delay_ms(50);  
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

void alternateBlink(uint8_t whichLED){
  LED_PORT = (1 << whichLED);
  _delay_ms(ANIMATION_TIME);
  LED_PORT = (1 << (whichLED+1));
  _delay_ms(ANIMATION_TIME);     
}

void doAnimation1(void){
  alternateBlink(0);
}
void doAnimation2(void){
  alternateBlink(2);
}
void doAnimation3(void){
  alternateBlink(4);
}
void doAnimation4(void){
  alternateBlink(6);
}



int main(void){
  uint8_t whichAnimation;
  init();
  blinkTest(3);

  while(1){			/* mainloop */

    switch(whichAnimation){
    case 1:
      doAnimation1();
      break;
    case 2:
      doAnimation2();
      break;
    case 3:
      doAnimation3();
      break;
    case 4:
      doAnimation4();
      break;
    }
    
    if (isButtonPressed()){
      /* advance index and wait for release */
      whichAnimation++;
      if (whichAnimation > 4){
	whichAnimation = 1;
      }
      /* Blink all LEDs until button is released */
      do {blinkTest(1);} 
      while(!isButtonReleased());
    }

  } /* end mainloop */
  return(0);
}
