
#include "main.h" 

// -------- Global Variables --------- //

// -------- Functions --------- //

int main(void) {
  // -------- Inits --------- //

  uint8_t i;
  clock_prescale_set(clock_div_1);                 /* CPU Clock: 8 MHz */
  initUSART();

  // ------ Event loop ------ //
  while (1) {



  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
