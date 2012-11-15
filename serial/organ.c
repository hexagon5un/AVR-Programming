/* 

Organ.c

A one-button organ

Version: 1.0

*/


// ------- Preamble -------- //
#include <avr/io.h>		
#define F_CPU 1000000UL	       
#include <util/delay.h>		
#include "common.h"
#include "foo.h"


int main(void){
  // -------- Inits --------- //
  initLED();
  flashLED();

  // ------ Event loop ------ //
  while(1){	
    bogus();
    

  }   /* End event loop */
  return(0);			
}





