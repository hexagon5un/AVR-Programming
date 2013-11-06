
/*
 *
 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "binaryMacro.h"


// -------- Global Variables --------- //

// -------- Functions --------- //

int main(void) {
  // -------- Inits --------- //

  initUSART();
  printString("Binary Macro Demo\r\n");

  printString("01010101 = ");
  printByte(B8(01010101));
  printString("\r\n");

  printString("00000001 = ");
  printByte(B8(00000001));
  printString("\r\n");

  printString("10101010 = ");
  printByte(B8(10101010));
  printString("\r\n");

  printString("10000000 = ");
  printByte(B8(10000000));
  printString("\r\n");

  return (0);                            /* This line is never reached */
}
