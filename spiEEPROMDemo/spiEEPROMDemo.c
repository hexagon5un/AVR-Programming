/* SPI EEPROM 25LC256 Demo */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"
#include "25LC256.h"

#define ARBITRARY_MEMORY_LOCATION  123

int main(void){
  uint16_t testNumber;

  // -------- Inits --------- //
  initSPI();
  initUSART();
  printString("\r\n====  EEPROM Memory Test ====\r\n");

  printString("This was in memory on reset: ");
  printByte(EEPROM_getByte(ARBITRARY_MEMORY_LOCATION));
  printString(" ");
  printByte(EEPROM_getByte(ARBITRARY_MEMORY_LOCATION+1));
  printString("\r\n");
  
  /* Start with a clean slate */
  /* Be patient, this can take a few seconds */
  set_bit(LED_DDR, LED0);
  set_bit(LED_PORT, LED0);
  printString("Erasing EEPROM, be patient...\r\n");
  EEPROM_clearAll();
  clear_bit(LED_PORT, LED0);

  printString("\r\nThese bytes should all be zero... ");
  printByte(EEPROM_getByte(0x0000));
  printString(" ");
  printByte(EEPROM_getByte(0x7fff));
  printString("\r\n");

  testNumber = 250;

  // ------ Event loop ------ //
  while(1){     

    // Display memory contents -- should start off 000, 000
    //  and then increase / change as we write to it
    printString("What's in memory?  ");
    printByte(EEPROM_getByte(ARBITRARY_MEMORY_LOCATION));
    printString(" ");
    printByte(EEPROM_getByte(ARBITRARY_MEMORY_LOCATION+1));
    printString("\r\n");
    
    // Write a value in to memory
    EEPROM_writeWord(ARBITRARY_MEMORY_LOCATION, testNumber);
    // Increment it
    testNumber++;

    _delay_ms(2000);

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

