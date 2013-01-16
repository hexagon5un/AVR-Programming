/* Includes */
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

/* Defines */
#define THRESHOLD   22	
/* How long to count as a press: this will vary with the 
   baseline capacitance of your setup.  Plotting the values
   under different circumstances will help you set this. */


/* Function Prototypes */

static inline void initTimer0(void);
static inline uint8_t pollSensor(void);

