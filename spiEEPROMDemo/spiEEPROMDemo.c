/* SPI EEPROM 25LC256 Demo */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"
#include "25LC256.h"

int main(void){
  uint8_t i;

  // -------- Inits --------- //
  initSPI();
  initUSART();
  printString("\r\n====  EEPROM Memory Test ====\r\n");

  /* Test write in some data */
  printString("default mode is writing disabled\r\n");
  printBinaryByte(EEPROM_readStatus()); 
  printString("\r\n");

  EEPROM_writeEnable(); /* make sure it's write-enabled */  	
  printString("set writing enabled\r\n");
  printBinaryByte(EEPROM_readStatus()); 
  printString("\r\n");

  printString("now re-select and start writing\r\n");
  SLAVE_SELECT;
  SPI_sendReceive(EEPROM_WRITE);
  EEPROM_send16BitAddress(123);
  /* Send some "data" */
  for (i=32; i<39; i++){
    SPI_sendReceive(i);
  }
  SLAVE_DESELECT;
  
  /* Wait for the write cycle to finish */
  printBinaryByte(EEPROM_readStatus()); 
  printString("  ... least significant bit is write-in-progres flag\r\n");

  while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)){;}

  printString("write flag reset after one write\r\n");
  printBinaryByte(EEPROM_readStatus());
  printString("\r\n");

  /* Read it back */
  printString("let's read and see if our data is there\r\n");
  SLAVE_SELECT;
  SPI_sendReceive(EEPROM_READ);
  EEPROM_send16BitAddress(120);
  for (i=0; i<15; i++){
    SPI_sendReceive(0);
    printByte(SPDR);
    printString("\r\n");
  }
  SLAVE_DESELECT;
  
  // ------ Event loop ------ //
  while(1){     
    
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

