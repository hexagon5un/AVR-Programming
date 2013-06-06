/* Reads LM75 Thermometer and Prints Value over Serial */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"
#include "i2c.h"

// -------- Defines -------- //    

#define LM75_ADDRESS_W 0b10010000
#define LM75_ADDRESS_R 0b10010001

// -------- Functions --------- //

int main(void){

  uint8_t tempHighByte, tempLowByte;

  // -------- Inits --------- //
  initUART();
  printString("\r\n====  i2c Thermometer  ====\r\n");
  initI2C();
  
  // ------ Event loop ------ //
  while(1){     
    
    i2cStart();
    /* Setup and send address, with read bit */
    i2cSend(LM75_ADDRESS_R);	
    /* Now receive two bytes of temperature */
    tempHighByte = i2cReadAck();	
    tempLowByte  = i2cReadNoAck();	
    i2cStop();

    // Print it out nicely over serial for now...
    printByte(tempHighByte);
    if (tempLowByte & _BV(7)){
      printString(".5\r\n");
    }
    else{
      printString(".0\r\n");
    }
    
    /* Once per second */
    _delay_ms(1000);
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

