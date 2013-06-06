
/* Support functions that otherwise make the main code more readable */

/* Includes */
#include <util/delay.h>         
#include <avr/io.h>             
#include "UART.h"

/* Function Prototypes in support.c */

void printMilliseconds(uint16_t value);
  /* Given a value in milliseconds, prints out how many seconds 
     you took over the serial port.  Does ascii conversion, prints
     decimal point, and drops extra leading zeros.
  */

void printComments(uint16_t value);
  /* Given a value in milliseconds, rates your reaction time */

void randomDelay(void);
  /* Waits for a "random" delay from 1 - 3.5 sec */
  /* Requires timer 1 initialized and running */
  /* It's not really random, but very hard to control
     like coin-flipping. */
