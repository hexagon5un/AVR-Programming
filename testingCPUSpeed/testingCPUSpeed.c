/*
 *
 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

#include "pinDefines.h"
#include "USART.h"

// -------- Global Variables --------- //

// -------- Functions --------- //

int main(void) {
  // -------- Inits --------- //

  uint8_t i;
	initUSART();
	LED_DDR = 0xff;

  // ------ Event loop ------ //
  while (1) {

	clock_prescale_set(clock_div_1);                /* CPU Clock: 8 MHz */
	for (i=0; i<10; i++){
		LED_PORT ^= (1<<LED0);	
		printString("Hi there.\r\n");
		_delay_ms(1000);
	}

	clock_prescale_set(clock_div_2);                /* CPU Clock: 8 MHz */
	for (i=0; i<10; i++){
		LED_PORT ^= (1<<LED0);	
		printString("Hi there.\r\n");
		_delay_ms(1000);
	}

	clock_prescale_set(clock_div_8);                /* CPU Clock: 8 MHz */
	for (i=0; i<10; i++){
		LED_PORT ^= (1<<LED0);	
		printString("Hi there.\r\n");
		_delay_ms(1000);
	}

	clock_prescale_set(clock_div_16);                /* CPU Clock: 8 MHz */ 
	for (i=0; i<10; i++){
		LED_PORT ^= (1<<LED0);	
		printString("Hi there.\r\n");
		_delay_ms(1000);
	}





	
	
	}                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
