/* Demonstrating the use of PROGMEM for storing long strings */

#include <avr/io.h>           /* Defines pins, ports, etc */
#define F_CPU 8000000UL       /* Sets up the chip speed for delay.h */
#include <util/delay.h>       /* Functions to waste time */
#define BAUDRATE  9600
#include "USART88.h"           /* uses BAUDRATE, so define it first */
#include "pinDefines.h"
#include "commonInits.h"

#include <avr/pgmspace.h>


/* Note that progmem strings need to be defined in the global namespace: outside of main */
const char myVeryLongString[] PROGMEM = "Hi there, this is a demonstration long string.\r\nThe kind that you wouldn't want to store in RAM.\r\n";
/* const is optional, but it prevents us from doing anything silly, 
   like trying to write to it. */


int main(void){

  char shortString[] = "Hi.\r\n";

  uint8_t i;
  const char* address;
  char oneCharacter;
  
  initLED();
  _delay_ms(100);
  initUART(); 
  sayOK();

  while(1){

    /* Here is a nice way of printing out entire strings... */
    for (i=0; i < sizeof(shortString)/sizeof(char); i++){ 
      transmitByte(shortString[i]);
    } 
    
    /* Now print out the same string, but from PROGMEM */
    for (i=0; i < sizeof(myVeryLongString)/sizeof(char); i++){ 
       address = &myVeryLongString[i]; 
       oneCharacter = pgm_read_byte(address); 
       transmitByte(oneCharacter);       
       // Note: transmitByte(pgm_read_byte(&(myVeryLongString[i]))); will also work 
     } 



    /* Delay for a while. */
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);

    LED_PORT ^= _BV(LED);

  }

}
