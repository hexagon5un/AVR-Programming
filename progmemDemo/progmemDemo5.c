/* Fourth step into using program memory */
/* Passing data array pointers to functions */

#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/pgmspace.h>
#include "USART.h"

const uint16_t myData[] PROGMEM = {1111,2222,3333,4444,5555,6666,7777,8888,9999,10000};

const char string1[] PROGMEM = "Hey, string one!\r\n";
const char string2[] PROGMEM = "Oh my, string two!\r\n";
const char* const stringIndex[] PROGMEM = {string1, string2};


void printData_Progmem(const uint16_t* dataPointer, uint8_t length){
  while (length){
    printWord((uint16_t) dataPointer); /* print out address */
    printString(":  ");
    printWord(pgm_read_word(dataPointer)); /* print out data */
    printString("\r\n");
    dataPointer++;			/* move to next byte */
    length--;			/* one less byte to go */
    _delay_ms(100); 		
  }
}
void printString_Progmem(const char* stringP){
  char oneLetter;
  while(( oneLetter = pgm_read_byte(stringP) )){
    transmitByte(oneLetter);
    stringP++;
  }
}

int main(void){
  initUSART();

  uint8_t myArray[] = {10,11,12};
  uint8_t*  p;
  uint8_t i;
  p = &myArray[0];
  for (i=0; i<sizeof(myArray); i++){
    printByte(*(p+i));
    printString("\r\n");
    _delay_ms(1000);    
  } 
  

  /* To use them: */
  char* stringPointer;
  /* Get the pointer to the string you want from PROGMEM */
  
  stringPointer = (char*) pgm_read_word(&stringIndex[0]);
  printString_Progmem(stringPointer);
  /* or */
  stringPointer = (char*) pgm_read_word(&stringIndex[1]);
  printString_Progmem(&stringPointer[0]);
  /* or */
  printString_Progmem(PSTR("And this string got inlined.\r\n"));

  while(1){      
    printData_Progmem(myData, sizeof(myData)/sizeof(myData[0]));
    printString("\r\n");
    _delay_ms(1000);
  }                   /* End event loop */
  return(0);          /* This line is never reached  */
}

