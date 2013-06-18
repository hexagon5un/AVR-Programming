/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //    
/* Allocate space in the EEPROM, associate pointers */
uint8_t EEMEM testVariable = 9;  /* with pre-init */
uint8_t EEMEM testByte;
uint16_t EEMEM testWord;
char EEMEM testString[23];


// -------- Functions --------- //

int main(void){

  // -------- Inits --------- //
 
  initUSART();
  uint8_t i;
  char letter;
  
  // Store the 8-bit value 5 in EEPROM 
  eeprom_update_byte(&testByte, 5);
  
  // Store the 16-bit value 0x1234 in EEPROM 
  eeprom_update_word(&testWord, 0x1234);
  
  // Store a character array (string) in EEPROM 
  char myString[] = "hello world.\r\n";
  eeprom_update_block(&myString, testString, sizeof(myString));

  
  transmitByte('\r');
  transmitByte('\n');
  printByte(eeprom_read_byte(&testVariable));
  transmitByte('\r');
  transmitByte('\n');
 
  for (i=0; i<sizeof(myString); i++){
    letter = eeprom_read_byte(testString + i);
    transmitByte(letter);
  }

  // ------ Event loop ------ //
  return(0);                  /* This line is never reached  */
}

