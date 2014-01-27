
#include "main.h" 

// -------- Global Variables --------- //

// -------- Functions --------- //

int main(void) {
  // -------- Inits --------- //
  
  // clock_prescale_set(clock_div_1);                 /* CPU Clock: 8 MHz */
  initUSART();
  printString("OK");

  // ------ Event loop ------ //
  while (1) {



  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
