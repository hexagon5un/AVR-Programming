/* Includes */
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>         

#include "pinDefines.h"
#include "macros.h"
#include "UART.h"

#include "support.h"

/* Function Prototypes in reactionTimer.c */

static inline void initTimer1(void);

