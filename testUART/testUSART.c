
#include <avr/io.h>		
#include <util/delay.h>	
#include "USART.h"
#include "pinDefines.h"
#include "hello.h"
#include "foo.h"

int main(void){
  
  initUART();
  hello();
  hello2();
  
  
  LED_DDR |= (1<<LED0);
  while(1){
    
    _delay_ms(100);
    transmitByte('\r');
    transmitByte('\n');
    LED_PORT |= (1<<LED0);
    printWord(12003);
    LED_PORT = 0;
    printWord(12003);
    LED_PORT |= (1<<LED0);
    transmitByte('1');
    transmitByte('2');
    transmitByte('0');
    transmitByte('0');
    transmitByte('3');
    LED_PORT = 0;
    
  }
}
