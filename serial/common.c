#include <avr/io.h>		
#define F_CPU 1000000UL	       
#include <util/delay.h>		

#include "common.h"

void flashLED(){
     LED_PORT |= (1 << LED);
    _delay_ms(FLASH_DELAY);
    LED_PORT &= ~(1 << LED);
}

void initLED(){
  // Set output mode
  LED_DDR |= (1 << LED);
  
  // Blink a few times to make sure all working (optional)
  uint8_t i = 3;
  while(i){
    flashLED();
    _delay_ms(100);
    i--;
  }
}

