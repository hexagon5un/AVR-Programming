#include <avr/io.h>
#include <avr/eeprom.h>
#include <UART.h>

#define STRING_MAXLEN         0x20	/* 32 characters */
#define STRING_ADDRESS        0x20
#define COUNTER_ADDRESS       0x10


int main(void){
  
  char myString[STRING_MAXLEN];
  char* eepromAddress = (char*) STRING_ADDRESS;
  uint16_t counter;

  initUART();

  while(1){
    // Read from EEPROM and print out
    eeprom_read_block(myString, eepromAddress, STRING_MAXLEN);
    counter = eeprom_read_word((uint16_t*) COUNTER_ADDRESS);
    printString("\r\nYour old favorite color is: ");
    printString(myString);

    // Take input, store in EEPROM
    printString("\r\n\r\n Type your new favorite color.  ");
    readString(myString, sizeof(myString)); 
           /* pass by address, function will change its values */
    eeprom_update_block(myString, eepromAddress, STRING_MAXLEN);
    counter++;
    printString("Thanks! \r\nYou've answered the same question ");
    printWord(counter);
    printString(" times.  \r\n");
    eeprom_update_word((uint16_t*) COUNTER_ADDRESS, counter);
  }
  
  return(0);
}

