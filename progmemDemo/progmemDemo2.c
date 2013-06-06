/* Second steps into using program memory */
/* Storing the addresses in pointers  */

#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/pgmspace.h>
#include "UART.h"

const  char myVeryLongString[] PROGMEM = "\r\nHi there, \
this is an example of a long string.\r\n\
The kind that you wouldn't want to store in RAM.\r\n";
const  uint16_t sixteenBits PROGMEM = 12345;

int main(void){
  initUART();

  const char* stringPointer;
  const uint16_t* wordPointer;
  uint8_t i;
  char oneLetter;

  stringPointer = &myVeryLongString[0]; /* address of first char */
  // stringPointer = myVeryLongString;     /* same as above */
  wordPointer = &sixteenBits;		/* address of first byte */

  while(1){      
    for (i = 0; i < sizeof(myVeryLongString); i++){
      oneLetter = pgm_read_byte( stringPointer + i );
      transmitByte(oneLetter);		
      _delay_ms(100);  /* slow it down to simulate typing effect :) */
    }
    _delay_ms(1000);

    printWord(pgm_read_word(wordPointer));
  }                   /* End event loop */
  return(0);          /* This line is never reached  */
}

