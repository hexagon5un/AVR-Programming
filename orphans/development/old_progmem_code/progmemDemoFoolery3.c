/* */
#include <avr/pgmspace.h> 
#include <util/delay.h>         
#include "pinDefines.h"
#include "USART.h"

char welcomeString[] PROGMEM = "Hello World! Hello World! Hello World! Hello World! Hello World!\r\n  This string is too long.\r\n";
char welcomeString2[] PROGMEM = "Hello World! Hello World! Hello World! Hello World! Hello World!\r\n  This string is too long.\r\n";

void printString_P(char *data){
  char oneLetter;
  while ((oneLetter = pgm_read_byte(data))){
    // transmitByte(oneLetter);
    data++;
  }
}

void printString_P2(char *data){
  uint8_t i=0;
  char thisLetter;
  while (thisLetter = pgm_read_byte(data + i)){
    // transmitByte(thisLetter);
    i++;
  }
}

void printString_P3(char *data){
  while (pgm_read_byte(data)){
    pgm_read_byte(data);
    //    transmitByte(pgm_read_byte(data));
    data++;
  }
}
void printString_P4(char *data){
  char oneLetter;
  while (*data){
    // transmitByte(*data);
    data++;
  }
}

int main(void){     
  initUSART();
  LED_DDR |= (1<<LED0);

  while(1){
    
    LED_PORT ^= (1<<LED0);
    /// ONE
    printString_P(welcomeString);
    LED_PORT ^= (1<<LED0);
    /// TWO
    printString_P2(&welcomeString2);
    LED_PORT ^= (1<<LED0);
    //printString_P4("hello\r\n");
    LED_PORT ^= (1<<LED0);
    _delay_ms(1000);    

  }
  return(0);
}

