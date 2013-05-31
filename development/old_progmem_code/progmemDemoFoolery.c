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
int main(void){
  
  // -------- Inits --------- //
  
  
  uint8_t myNumber;
  uint8_t myOtherNumber;
  uint16_t address;

  LED_DDR |= (1<<LED0);
  _delay_ms(100);
  initUSART(); 
  printString("Hello?\r\n");
  myNumber = 12;
  myOtherNumber = 234;
  
  // ------ Event loop ------ //    
  while(1){
    
    address = &myNumber;

    printWord(address);
    printString("\r\n");
    printWord(&myOtherNumber);
    printString("\r\n");
   
    _delay_ms(1000);
    LED_PORT ^= (1<<LED0);
    myNumber++;

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}


