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

void transmitString_P(const char *data){
  while (pgm_read_byte(data)){
    transmitByte(pgm_read_byte(data));
    data++;
  }
}

int main(void){

  char shortString[] = "Hi.\r\n";
//  char myVeryLongString2[] = "Hi there, this is a demonstration long string.\r\nThe kind that you would";
  
  uint8_t i;
  
  initLED();
  _delay_ms(100);
  initUART(); 
  sayOK();

  while(1){

    /* Here is a nice way of printing out entire strings... */
    for (i=0; i < sizeof(shortString)/sizeof(char); i++){ 
      transmitByte(shortString[i]);
    } 

   
    transmitString_P(myVeryLongString);
    


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
