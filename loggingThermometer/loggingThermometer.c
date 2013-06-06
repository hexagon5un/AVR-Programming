/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "UART.h"

#include "i2c.h"		/* for i2c functions */
#include "25LC256.h"		/* for EEPROM specific */

// -------- Defines --------- //

#define LM75_ADDRESS_W 0b10010000
#define LM75_ADDRESS_R 0b10010001

#define CURRENT_LOCATION_POINTER  0 
/* where to store a pointer to the current reading in EEPROM */
#define SECONDS_POINTER           2        
/* store seconds-delay value here */
#define MEMORY_START              4        
/* where to start logging temperature values */
#define MENU_DELAY                5
/* seconds to wait before bypassing main menu */

// -------- Functions --------- //

static inline void printTemperature(uint8_t tempReading){
  /* temperature stored as 2x Celcius */
  printByte((tempReading >> 1));
  if (tempReading & 1){
    printString(".5\r\n");
  }
  else{
    printString(".0\r\n");
  }
}

int main(void){
  uint16_t secondsDelay;	/* how long to wait between readings */
  uint16_t currentMemoryLocation; /* where are we in EEPROM? */
  uint16_t i;			  /* used to count memory locations */
  uint8_t tempHighByte, tempLowByte, temperatureByte; /* from LM75 */
  uint8_t enterMenu;	/* logical flag */
  uint8_t configRegister;

  // -------- Inits --------- //
  initSPI();
  initI2C();
  initUART();
  set_bit(LED_DDR, LED0);
  
  /* Load up last values from EEPROM */
  secondsDelay = EEPROM_readWord(SECONDS_POINTER);

  /* Delay to allow input to enter main menu */
  printString("*** Press [m] within ");
  printByte(MENU_DELAY);
  printString(" seconds to enter menu. ***\r\n ");
  
  for (i=0; i<MENU_DELAY; i++){
    _delay_ms(1000);
  }
  
  if (bit_is_set(UCSR0A, RXC0) && (UDR0 == 'm')){ 
    enterMenu = 1;
  }
  else{
    enterMenu = 0;
  }
  
  while (enterMenu){
    printString("\r\n====[  Logging Thermometer ]====\r\n  ");
    currentMemoryLocation = EEPROM_readWord(CURRENT_LOCATION_POINTER);
    print16bits(currentMemoryLocation - MEMORY_START);
    printString(" readings in log.\r\n  ");
    print16bits(secondsDelay);
    printString(" seconds between readings.\r\n");
    printString(" [<] to shorten sample delay time\r\n");
    printString(" [>] to increase sample delay time\r\n");
    printString(" [?] to reset delay time to 60 sec\r\n");
    printString(" [d] to print out log over serial\r\n");
    printString(" [e] to erase memory\r\n");
    printString(" [s] to start logging\r\n\r\n");
    
    switch(receiveByte()){
    case 'd':
      SLAVE_SELECT;
      SPI_tradeByte(EEPROM_READ);
      EEPROM_send16BitAddress(MEMORY_START);
      for(i=MEMORY_START; i<currentMemoryLocation; i++){
	SPI_tradeByte(0);
	printTemperature(SPDR);
      }
      SLAVE_DESELECT;
      break;
    case '<':
      if (secondsDelay >= 10){
	secondsDelay-=5;
	EEPROM_writeWord(SECONDS_POINTER, secondsDelay);
      }
      break;
    case '>':
      if (secondsDelay < 65000){
	secondsDelay+=5;
	EEPROM_writeWord(SECONDS_POINTER, secondsDelay);
      }
      break;
    case '?':
      secondsDelay=60;
      EEPROM_writeWord(SECONDS_POINTER, secondsDelay);
      break;
    case 'e':
      printString("Clearing EEPROM, this could take a few seconds.\r\n");
      EEPROM_clearAll();
      EEPROM_writeWord(CURRENT_LOCATION_POINTER, MEMORY_START);	
      EEPROM_writeWord(SECONDS_POINTER, secondsDelay);	
      break;
    case 's':
      printString("OK, logging...\r\n");
      enterMenu = 0;
      break;
    default:
      printString("Sorry, didn't understand that.\r\n");
    }
  }
    
  // ------ Event loop ------ //
  while(1){
    
    currentMemoryLocation = EEPROM_readWord(CURRENT_LOCATION_POINTER);
    
    /* Get Temp from thermometer */
    i2cStart();                /* Setup and send address, with read bit */
    i2cSend(LM75_ADDRESS_R);	
    tempHighByte = i2cReadAck(); /* two bytes of temperature */	
    tempLowByte  = i2cReadNoAck();	
    i2cStop();
    temperatureByte = (tempHighByte << 1) | (tempLowByte >> 7);
    /* temperatureByte now contains 2x the temperature in Celcius */
    printTemperature(temperatureByte);           /* serial output */
    
    /* Save the new temperature value */
    EEPROM_writeByte(currentMemoryLocation, temperatureByte); 
    
    /* move on to next location and record new position
       if not already at the end of memory */
    if (currentMemoryLocation < EEPROM_BYTES_MAX){ 
      currentMemoryLocation++;	
      EEPROM_writeWord(CURRENT_LOCATION_POINTER, currentMemoryLocation);
    }
    
    /*  delay */
    for (i=0; i<secondsDelay; i++){
      _delay_ms(1000);
      toggle_bit(LED_PORT, LED0); /* blink to show working */
    }

  }    /* End event loop */
return(0);                  /* This line is never reached  */
}


