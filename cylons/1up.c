/* 
  POV demo by Patrick Roanhouse
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  1000000UL
#include <util/delay.h>
#define DELAYTIME 2		/* ms */

char Star1UP[] = {
  0b10000100,
  0b01101100,
  0b01111110,
  0b00011111,
  0b01111110,
  0b01101100,
  0b10000100,
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000010,
  0b11111111,
  0b11111111,
  0b10000000,
  0b00000000,
  0b00000000,
  0b01111111,
  0b11111111,
  0b10000000,
  0b11111111,
  0b01111111,
  0b00000000,
  0b00000000,
  0b11111111,
  0b11111111,
  0b00110011,
  0b00110011,
  0b00011110,
  0b00011110,
};


int main(void){
  uint8_t i;

  DDRB = 0xff;              /* all output */

  while(1){			/* mainloop */
       for (i = 0; i <= sizeof(Star1UP) / sizeof(char); ++i) {
      /* sizeof(Star1UP) returns the number of bits in our array,
       sizeof(char) is the length of each character (in bits). 
       Dividing them yields the number of characters.
      */
      PORTB = Star1UP[i];	
      _delay_ms(DELAYTIME);
    }

    PORTB = 0;			/* blank for gap between repetitions */
    _delay_ms(5*DELAYTIME);

  } /* end mainloop */
  return(0);
}
