/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //    

// -------- Functions --------- //
/* Note that progmem strings need to be defined in the global namespace: outside of main */
const char myVeryLongString[] PROGMEM = "Hi there, this is a demonstration long string.\r\nThe kind that you wouldn't want to store in RAM.\r\n";
/* const is optional, but it prevents us from doing anything silly, 
   like trying to write to it. */

int main(void){

  // -------- Inits --------- //
  
  const char shortString[] = "Hi.\r\n";
  
  uint8_t i;
  const char* address;
  char oneCharacter;
  
  LED_DDR |= (1<<LED0);
  _delay_ms(100);
  initUSART(); 
  
  // ------ Event loop ------ //    
  while(1){
    
    /* Here is a nice way of printing out entire strings... */
    printString(shortString);
    
    /* Now print out the same string, but from PROGMEM */
    for (i=0; i < sizeof(myVeryLongString); i++){ 
      //      address = &myVeryLongString[i]; 
      //oneCharacter = pgm_read_byte(address); 
      //transmitByte(oneCharacter);       
      // Note: transmitByte(pgm_read_byte(&myVeryLongString[i])); will also work 
      transmitByte(pgm_read_byte(&myVeryLongString[i]));
    } 
    /* Delay for a while. */
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    
    LED_PORT ^= (1<<LED0);
    
  
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

