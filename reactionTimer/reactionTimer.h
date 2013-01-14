/* Includes */
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>         

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#include "stringIO.h"

/* Function Prototypes */

static inline void initTimer1(void);

