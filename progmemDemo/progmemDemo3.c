/* Third step into using program memory */
/* Passing pointers to functions */

#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/pgmspace.h>
#include "USART.h"

const  char myVeryLongString1[] PROGMEM = "\r\nHi there, \
this is an example of a long string.\r\n\
The kind that you wouldn't want to store in RAM.\r\n";
const  char myVeryLongString2[] PROGMEM = "All work and no play \
makes Jack something something.\r\n";

void printString_Progmem(const char* stringP){
  char oneLetter;
  while(( oneLetter = pgm_read_byte(stringP) )){
    transmitByte(oneLetter);
    stringP++;
    _delay_ms(100); 		/* only b/c it's cute */
  }
}

int main(void){
  initUSART();
  while(1){      
    printString_Progmem(&myVeryLongString1[0]); 
    printString_Progmem(&myVeryLongString1[50]); 
    printString_Progmem(myVeryLongString2);	
    _delay_ms(1000);
  }                   /* End event loop */
  return(0);          /* This line is never reached  */
}

