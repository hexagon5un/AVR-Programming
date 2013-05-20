/* SPI EEPROM 25LC256 Demo */
#include <avr/io.h>             
#include "pinDefines.h"
#include "macros.h"

/* Which pin selects EEPROM as slave? */
#define SLAVE_SELECT    clear_bit(SPI_SS_PORT, SPI_SS)
#define SLAVE_DESELECT  set_bit(SPI_SS_PORT, SPI_SS)

// Instruction Set -- from data sheet
#define EEPROM_READ      0b00000011	/* read memory */
#define EEPROM_WRITE     0b00000010	/* write to memory */

#define EEPROM_WRDI      0b00000100	/* write disable */
#define EEPROM_WREN      0b00000110	/* write enable */

#define EEPROM_RDSR      0b00000101	/* read status register */
#define EEPROM_WRSR      0b00000001	/* write status register */

// EEPROM Status Register Bits -- from data sheet
// Use these to parse status register reads
#define EEPROM_WRITE_IN_PROGRESS    0      
#define EEPROM_WRITE_ENABLE_LATCH   1	
#define EEPROM_BLOCK_PROTECT_0	    2	
#define EEPROM_BLOCK_PROTECT_1	    3	

void initSPI(void);
/* Init SPI to run EEPROM with phase, polarity = 0,0 */

void SPI_sendReceive(uint8_t byte);
/* Generic.  Just loads up HW SPI register and waits */

void EEPROM_send16BitAddress(uint16_t address);
/* splits 16-bit address into 2 bytes, sends both */

uint8_t EEPROM_readStatus(void);
/* reads the EEPROM status register */

void EEPROM_writeEnable(void);
/* helper: sets EEPROM write enable */

