/* 
  Space Invaders POV demo by Elliot Williams
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  1000000UL
#include <util/delay.h>
#define DELAYTIME 2		/* ms */

char invaders1[] = {
  0b01110000,
  0b00011000,
  0b11111101,
  0b10110110,
  0b00111100,
  0b00111100,
  0b00111100,
  0b10110110,
  0b11111101,
  0b00011000,
  0b01110000
};

char invaders2[] = {
  0b00001110,
  0b00011000,
  0b10111101,
  0b01110110,
  0b00111100,
  0b00111100,
  0b00111100,
  0b01110110,
  0b10111101,
  0b00011000,
  0b00001110
};


void main(void){
  char input;

  DDRB = 0xff;              /* all output */

  while(1){			/* mainloop */
    
    for (int i = 0; i < sizeof(invaders1) / sizeof(char); ++i) {
      /* sizeof(Star1UP) returns the number of bits in our array,
	 sizeof(char) is the length of each character (in bits). 
	 Dividing them yields the number of characters.
      */
      PORTB = invaders1[i];	
      _delay_ms(DELAYTIME);
    }
    
    PORTB = 0;			/* blank for gap between repetitions */
    _delay_ms(5*DELAYTIME);
    
    for (int i = 0; i < sizeof(invaders2) / sizeof(char); ++i) {
      PORTB = invaders2[i];	
      _delay_ms(DELAYTIME);
    }
    
    PORTB = 0;			/* blank for gap between repetitions */
    _delay_ms(5*DELAYTIME);
    
  } /* end mainloop */
  
}
