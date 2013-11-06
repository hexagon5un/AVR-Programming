
// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#define BAUDRATE  9600
#include "USART.h"

#define LED_PORT  PORTB
#define LED_DDR   DDRB


int main(void) {
  char serialCharacter;

  // -------- Inits --------- //
  // Set output mode for whole B bank
  LED_DDR = 0xff;
  initUSART();
  printString("Hello World!\r\n");

  // ------ Event loop ------ //
  while (1) {

    serialCharacter = receiveByte();
    transmitByte(serialCharacter);
    LED_PORT = serialCharacter;
                           /* display ascii/numeric value of character */

  }                                                  /* End event loop */
  return (0);
}
