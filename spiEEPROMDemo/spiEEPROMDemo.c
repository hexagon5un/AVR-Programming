/* SPI EEPROM 25LC256 Demo */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //    

// -------- Functions --------- //
#define SS PB2

static inline void initSPI(void){

  set_bit(DDRB, SS);		/* set SS output */
  set_bit(PORTB, SS); 		/* set high to disable initially */
  
  set_bit(DDRB, PB3);		/* output on MOSI */
  set_bit(PORTB, PB4);		/* pullup on MISO */
  set_bit(DDRB, PB5);		/* output on SCK */  

  /* Don't have to set phase, polarity b/c 
     default works with 25LCxxx chips */
  SPCR |= _BV(SPR1);            /* div 16, safer for breadboards */
  SPCR |= _BV(MSTR);            /* clockmaster */
  SPCR |= _BV(SPE);             /* enable */
}


// Instruction Set
#define EEPROM_READ      0b00000011	/* read memory */
#define EEPROM_WRITE     0b00000010	/* write to memory */

#define EEPROM_WRDI      0b00000100	/* write disable */
#define EEPROM_WREN      0b00000110	/* write enable */

#define EEPROM_RDSR      0b00000101	/* read status register */
#define EEPROM_WRSR      0b00000001	/* write status register */

#define EEPROM_SELECT    clear_bit(PORTB, SS)
#define EEPROM_DESELECT  set_bit(PORTB, SS)


static inline void SPI_sendReceive(uint8_t byte){
  SPDR = byte;			/* load up byte */
  loop_until_bit_is_set(SPSR, SPIF);
}

static inline void EEPROM_writeEnable(void){
  EEPROM_SELECT;
  SPI_sendReceive(EEPROM_WREN); /* send write enable */
  EEPROM_DESELECT;
}

static inline void EEPROM_writeDisable(void){
  EEPROM_SELECT;
  SPI_sendReceive(EEPROM_WRDI); /* send write disable */
  EEPROM_DESELECT;
}

static inline uint8_t EEPROM_readStatus(void){
  EEPROM_SELECT;
  SPI_sendReceive(EEPROM_RDSR); /* read status command */
  SPI_sendReceive(0);	/* clock out eight bits */
  EEPROM_DESELECT;
  return(SPDR);
}

static inline void printBinaryByte(uint8_t byte){
  uint8_t bit = 8;
  while(bit){
    bit--;
    if ( (1<<bit) & byte ){
      transmitByte('1');
    }
    else{
      transmitByte('0');
    }
  }
  transmitString("\r\n");
}

static inline uint8_t nibbleToHex(uint8_t nibble){
  if (nibble < 10){
    return('0'+nibble);
  }
  else{
    return('A' + nibble - 10);
  }
}

static inline void printHexByte(uint8_t byte){
  uint8_t nibble;
  nibble = (byte & 0b11110000) >> 4;
  transmitByte(nibbleToHex(nibble));
  nibble = byte & 0b00001111;
  transmitByte(nibbleToHex(nibble));
  transmitString("\r\n");
}

static inline void transmitAddress(uint16_t address){
  SPI_sendReceive((uint8_t) (address >> 8)); /* most significant byte */
  SPI_sendReceive((uint8_t) address);	   /* least significant byte */
}


int main(void){
  uint8_t i;
  uint8_t spiByte;

  // -------- Inits --------- //
  initSPI();
  initUSART();
  sayOK();
  _delay_ms(1);
  
  /* EEPROM_writeDisable(); */
  /* spiByte = EEPROM_readStatus(); */
  /* printBinaryByte(spiByte); */
  
  /* EEPROM_writeEnable(); */
  /* spiByte = EEPROM_readStatus(); */
  /* printBinaryByte(spiByte); */

  /* Test write in some data */
  EEPROM_writeEnable();
  EEPROM_SELECT;
  SPI_sendReceive(EEPROM_WRITE);
  transmitAddress(1005);
  /* Send some "data" */
  for (i=20; i<28; i++){
    SPI_sendReceive(i);
  }
  EEPROM_DESELECT;
  
  /* Wait for the write cycle to finish up */
  while(EEPROM_readStatus() & _BV(1)){;}

  /* Read it back */
  EEPROM_SELECT;
  SPI_sendReceive(EEPROM_READ);
  transmitAddress(1000);
  for (i=0; i<32; i++){
    SPI_sendReceive(0);
    printByte(SPDR);
    transmitString("\r\n");
  }
  EEPROM_DESELECT;
  
  
  // ------ Event loop ------ //
  while(1){     
    
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

