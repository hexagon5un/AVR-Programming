#include <avr/io.h>
#include <avr/eeprom.h>
#include <USART.h>

#define STRING_LEN   80

// Define EEMEM variables
uint8_t eepromCounter EEMEM = 0;
char eepromString[STRING_LEN] EEMEM = "Welcome to the EEMEM Demo.\r\n";
uint16_t eepromWord EEMEM = 12345;

int main(void) {

  initUSART();
  char ramString[STRING_LEN];
  uint8_t counter;

  while (1) {
    printString("\r\n------------------\r\n");
    eeprom_read_block(ramString, eepromString, STRING_LEN);
    printString(ramString);

    printString("\r\nThe counter reads: ");
    counter = eeprom_read_byte(&eepromCounter);
    printByte(counter);

    printString("\r\nMy uint16_t value is: ");
    printWord(eeprom_read_word(&eepromWord));

    printString("\r\n   Enter a new introduction string below:\r\n");
    readString(ramString, STRING_LEN);
    eeprom_update_block(ramString, eepromString, STRING_LEN);
    counter++;
    eeprom_update_byte(&eepromCounter, counter);
  }
  return (0);
}
