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

  uint8_t a;

  // -------- Inits --------- //
  initUART();
  sayOK();  

  // ------ Event loop ------ //
  while(1){     

    
    
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

