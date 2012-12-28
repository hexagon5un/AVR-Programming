/* Includes */
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

/* Defines */
#define THRESHOLD 10		/* how long to count as a press */
/* note, shouldn't be below 8, it needs that long just for the code */
/* but unless you're using a big antenna (lots of metal) near 8 is good */

/* Global Variables */


/* Function Prototypes */

static inline void initTimer0(void);

static inline uint8_t pollSensor(void);
