#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>


// -------- Global Variables --------- //
volatile uint16_t ticks;        /* for system tick clock */

// -------- Function Prototypes ------ //

/* Configure timer0 to increment the global variable "ticks"
 * once per roughly every 10 ms (at 8MHz CPU clock rate) */ 
void initTicks(void);

/* Simple _delay function equivalent.  In clock ticks,
 * so it's 100 ticks per approximate second.  */ void sleepDelay(uint16_t numTicks);

 
