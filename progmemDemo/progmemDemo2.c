// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //    

// -------- Functions --------- //
/* Note that progmem strings need to be defined in the global namespace: outside of main */

char string1[] PROGMEM = "Hi there, this is a demonstration long string.\r\n";
char string2[] PROGMEM = "The kind that you wouldn't want to store in RAM.\r\n";
char* progmemStrings[] = {string1, string2};

//#define INTRO      0
//#define RATIONALE  1
enum {
  INTRO, 
  RATIONALE
};


void printString_P(char *data){
  char oneLetter;
  while (oneLetter = pgm_read_byte(data)){
    transmitByte(oneLetter);
    data++;
  }
}

int main(void){

  // -------- Inits --------- //
  
  uint8_t i;
  
  LED_DDR |= (1<<LED0);
  _delay_ms(100);
  initUSART(); 
  
  // ------ Event loop ------ //    
  while(1){

    for (i=0; i<2; i++){
      printString_P(progmemStrings[i]);
      _delay_ms(1000);
      LED_PORT ^= _BV(LED0);
    }
    printString_P(progmemStrings[RATIONALE]);
    //printString(progmemStrings[RATIONALE]);

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

