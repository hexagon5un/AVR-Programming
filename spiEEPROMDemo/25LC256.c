#include "25LC256.h"

void initSPI(void){
  set_bit(SPI_SS_DDR, SPI_SS);	/* set SS output */
  set_bit(SPI_SS_PORT, SPI_SS);	/* start off not selected (high) */
  
  set_bit(SPI_MOSI_DDR, SPI_MOSI);		/* output on MOSI */
  set_bit(SPI_MISO_PORT, SPI_MISO);		/* pullup on MISO */
  set_bit(SPI_SCK_DDR, SPI_SCK);		/* output on SCK */  

  /* Don't have to set phase, polarity b/c 
     default works with 25LCxxx chips */
  set_bit(SPCR, SPR1);            /* div 16, safer for breadboards */
  set_bit(SPCR, MSTR);            /* clockmaster */
  set_bit(SPCR, SPE);             /* enable */
}

void SPI_tradeByte(uint8_t byte){
  SPDR = byte;			/* SPI starts sending immediately */
  loop_until_bit_is_set(SPSR, SPIF); /* wait until done */
  /* SPDR now contains the received byte */
}

void EEPROM_send16BitAddress(uint16_t address){
  SPI_tradeByte((uint8_t) (address >> 8)); /* most significant byte */
  SPI_tradeByte((uint8_t) address);	   /* least significant byte */
}

uint8_t EEPROM_readStatus(void){
  SLAVE_SELECT;
  SPI_tradeByte(EEPROM_RDSR); 
  SPI_tradeByte(0);	 /* clock out eight bits */
  SLAVE_DESELECT;
  return(SPDR);  	 /* return the result */
}

void EEPROM_writeEnable(void){
  SLAVE_SELECT;
  SPI_tradeByte(EEPROM_WREN); 
  SLAVE_DESELECT;
}

uint8_t EEPROM_readByte(uint16_t address){
  SLAVE_SELECT;
  SPI_tradeByte(EEPROM_READ);
  EEPROM_send16BitAddress(address);
  SPI_tradeByte(0);
  SLAVE_DESELECT;
  return(SPDR);
}

uint16_t EEPROM_readWord(uint16_t address){
  uint16_t eepromWord;
  SLAVE_SELECT;
  SPI_tradeByte(EEPROM_READ);
  EEPROM_send16BitAddress(address);
  SPI_tradeByte(0);
  eepromWord = SPDR;
  eepromWord = (eepromWord << 8); /* most-sig bit */
  SPI_tradeByte(0);
  eepromWord += SPDR;              /* least-sig bit */
  SLAVE_DESELECT;
  return(eepromWord);
}

void EEPROM_writeByte(uint16_t address, uint8_t byte){
  EEPROM_writeEnable();
  SLAVE_SELECT;
  SPI_tradeByte(EEPROM_WRITE);
  EEPROM_send16BitAddress(address);
  SPI_tradeByte(byte);
  SLAVE_DESELECT;
  while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)){;}
}

void EEPROM_writeWord(uint16_t address, uint16_t word){
  EEPROM_writeEnable();
  SLAVE_SELECT;
  SPI_tradeByte(EEPROM_WRITE);
  EEPROM_send16BitAddress(address);
  SPI_tradeByte((uint8_t) (word >> 8));
  SPI_tradeByte((uint8_t) word);
  SLAVE_DESELECT;
  while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)){;}
}

void EEPROM_clearAll(void){
  uint8_t i;
  uint16_t pageAddress=0;
  while(pageAddress < EEPROM_BYTES_MAX){
    EEPROM_writeEnable();
    SLAVE_SELECT;
    SPI_tradeByte(EEPROM_WRITE);
    EEPROM_send16BitAddress(pageAddress);
    for(i=0; i<EEPROM_BYTES_PER_PAGE; i++){
      SPI_tradeByte(0);
    }
    SLAVE_DESELECT;
    pageAddress += EEPROM_BYTES_PER_PAGE;
    while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)){;}
  }
}
