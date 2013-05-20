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
#define SS PB1

static inline void initSPI(void){

  SPCR |= _BV(MSTR);            /* clockmaster! */
  SPCR |= _BV(SPR0);            /* div 16  -- no reason */
  SPCR |= _BV(CPOL) | _BV(CPHA); /* default memory is to mode 3 */
  SPCR |= _BV(SPE);             /* enable */

  set_bit(DDRB, SS);		/* set SS output */
  set_bit(PORTB, SS); 		/* set high */

  set_bit(DDRB, PB2);		/* set SS to output */
  set_bit(PORTB, PB2);		/* set SS to output */

  set_bit(DDRB, PB3);		/* output on MOSI */
  set_bit(PORTB, PB4);		/* pullup on MISO */
  set_bit(DDRB, PB5);		/* output on SCK */  
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
  //while(!(SPSR & (1<<SPIF)));	/* wait until SPI done */
  loop_until_bit_is_set(SPSR, SPIF);
}

static inline void EEPROM_writeEnable(void){
  EEPROM_SELECT;
  SPI_sendReceive(EEPROM_WREN); /* send write enable */
  EEPROM_DESELECT;
}

static inline void EEPROM_writeDisable(void){
  EEPROM_SELECT;
  SPI_sendReceive(EEPROM_WRDI); /* send write enable */
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
  nibble = (byte & 0b00001111) >> 4;
  transmitByte(nibbleToHex(nibble));
  nibble = byte & 0b00001111;
  transmitByte(nibbleToHex(nibble));
  transmitString("\r\n");
}


int main(void){
  uint8_t i;
  uint8_t spiByte;

  // -------- Inits --------- //
  set_bit(DDRB, PB0);
  set_bit(PORTB, PB0);
  _delay_ms(100);
  clear_bit(PORTB, PB0);

  initSPI();
  initUSART();
  sayOK();
  _delay_ms(1);
  
  EEPROM_writeDisable();
  spiByte = EEPROM_readStatus();
  printBinaryByte(spiByte);
  
  EEPROM_writeEnable();
  spiByte = EEPROM_readStatus();
  printBinaryByte(spiByte);
  

  // ------ Event loop ------ //
  while(1){     
    
    for (i=0; i<10; i++){
      set_bit(PORTB, PB0);
      _delay_ms(100);
      clear_bit(PORTB, PB0);
      _delay_ms(100);
    }


  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

