/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //    

// -------- Functions --------- //

int main(void){

  // -------- Inits --------- //
  initUSART();
  printString("\r\n====  i2c Thermometer  ====\r\n");


  // ------ Event loop ------ //
  while(1){     
    
    
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

