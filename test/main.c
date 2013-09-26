/*

Main.c

Describe what your program does here.

Version: 1.0

*/


// ------- Preamble -------- //
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

#define F_CPU 1000000UL
#include <util/delay.h>
// #define BAUDRATE  9600
// #include "USARTx8.h"




int main(void) {
  // -------- Inits --------- //


  // ------ Event loop ------ //
  while (1) {



  }                                                  /* End event loop */
  return (0);
}
