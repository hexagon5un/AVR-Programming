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
uint8_t EEMEM testVariable = 9;

// -------- Functions --------- //

int main(void){

  // -------- Inits --------- //
 
  initUSART();
  
  
  /* // Store the 8-bit value 5 in EEPROM slot 0 */
  /* uint8_t* address = 0; */
  /* eeprom_update_byte(address, 5); */
  
  /* // Store the 16-bit value 12345 in EEPROM slots 1 and 2: */
  /* eeprom_update_word((uint16_t*) 1, 12345); */
  
  /* // Store a character array (string) in EEPROM slot 3-15: */
  /* char myString[] = "hello world.";  */
  /* eeprom_update_block(&myString, (void*) 3, sizeof(myString)); */

  

  printByte(eeprom_read_byte(&testVariable));
   
  // ------ Event loop ------ //
  return(0);                  /* This line is never reached  */
}

