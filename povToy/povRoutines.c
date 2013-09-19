#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"	        /* hardware hookup defines */
#include "thinFont.h"		/* for font character array */

#include "povRoutines.h"	/* for function prototypes */

void pause(void){
  PORTB = 0;                    
  _delay_ms(5*DELAYTIME);
}

void POVChar(char character){
  uint8_t column;
  /* Display character, column-by-column */
  for (column = 0; column < 5; column++){ 
    LED_PORT = characterArray[character - ' '][column];
    _delay_ms(DELAYTIME);
  }  
}

void POVString(char myString[]){
  uint8_t letterNum=0;

  while (myString[letterNum]) {   /* repeat until end of string */
    POVChar(myString[letterNum]);
    // Delay between characters, go to next letter
    LED_PORT = 0;
    _delay_ms(DELAYTIME);
    _delay_ms(DELAYTIME);
    _delay_ms(DELAYTIME);
    letterNum++;
  } 
  pause();			/* then leave a big blank */
}

void POVDisplay(uint8_t povData[], uint8_t numberRows){
  uint8_t i;
  for (i = 0; i < numberRows; ++i) {
    LED_PORT = povData[i];    
    _delay_ms(DELAYTIME); 
  }
  pause();
}


void cylonPOV(void){
  uint8_t whichLED = 0;
  while(whichLED < 7){
    LED_PORT = (1 << whichLED);
    _delay_ms(DELAYTIME); 
    whichLED++;
  }
  while(whichLED > 0){
    LED_PORT = (1 << whichLED);
    _delay_ms(DELAYTIME); 
    whichLED--;
  }
}
