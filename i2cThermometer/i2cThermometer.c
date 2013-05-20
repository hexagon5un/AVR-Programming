/* Reads LM75 Thermometer and Prints Value over Serial */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //    

#define LM75_ADDRESS_W 0b10010000
#define LM75_ADDRESS_R 0b10010001

// -------- Functions --------- //

static inline void initI2C(void){
  set_bit(I2C_SDA_PORT, I2C_SDA); /* pullup for SDA line */
  set_bit(I2C_SCL_PORT, I2C_SCL); /* pullup for SCL line */
  TWBR = 32;			  /* set bit rate, see p. 242 */
			    /* 8MHz / (16+2*TWBR*1) ~= 100kHz */
  TWCR |= (1 << TWEN);		 /* enable */
}

static inline void i2cWaitForComplete(void){
  loop_until_bit_is_set(TWCR, TWINT);
}

static inline void i2cStart(void){
  TWCR = (_BV(TWINT)|_BV(TWEN)|_BV(TWSTA));
  i2cWaitForComplete();
}

static inline void i2cStop(void){
  TWCR = (_BV(TWINT)|_BV(TWEN)|_BV(TWSTO));
}

static inline uint8_t i2cReadAck(void){
  TWCR = (_BV(TWINT)|_BV(TWEN)|_BV(TWEA)); 
  i2cWaitForComplete();
  return(TWDR);
}

static inline uint8_t i2cReadNoAck(void){
  TWCR = (_BV(TWINT)|_BV(TWEN)); 
  i2cWaitForComplete();
  return(TWDR);
}

static inline void i2cSend(uint8_t data){
  TWDR = data;
  TWCR = (_BV(TWINT)|_BV(TWEN)); /* init and enable */
  i2cWaitForComplete();
}

int main(void){

  uint8_t tempHighByte, tempLowByte;

  // -------- Inits --------- //
  initUSART();
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

