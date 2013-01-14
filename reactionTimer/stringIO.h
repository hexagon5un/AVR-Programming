
/* Support functions that otherwise make the main code more readable */

#include <util/delay.h>         
#include <avr/io.h>             
#include "USART.h"

// ----------------- Function Prototypes ---------------- //
void print16(uint16_t value);
/* 
   Takes a 16-bit integer, prints out the ascii string that corresponds
   to the value / 1000, complete with decimal point.
*/

void printComments(uint16_t value);
/* 
   Prints out cute messages over the serial port depending on how fast/slow
   your reaction time was.  
*/

void randomDelay(void);
/* 
   Uses the timer value as a "random" number for the
   basis of a random delay.
 */
