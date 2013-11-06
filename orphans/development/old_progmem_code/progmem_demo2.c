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
const char string1[] PROGMEM = "Hi there, this is a demonstration long string.\r\n";
const char string2[] PROGMEM = "The kind that you wouldn't want to store in RAM.\r\n";
const char* progmemStrings[] = {string1, string2};


void transmitString_P(const char *data){
  while (pgm_read_byte(data)){
    transmitByte(pgm_read_byte(data));
    data++;
  }
}

int main(void){

  uint8_t i, j;
  uint16_t address;
  char oneCharacter;
  
  initLED();
  _delay_ms(100);
  initUART(); 
  sayOK();

  while(1){
    for (i=0; i<2; i++){
      transmitString_P(progmemStrings[i]);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);

      LED_PORT ^= _BV(LED);
    }
    



    /* Delay for a while. */
 
  }

}
