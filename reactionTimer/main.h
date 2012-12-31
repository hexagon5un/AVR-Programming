/* Includes */
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>         
#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

/* Function Prototypes */

static inline void initTimer1(void);
static inline void print16(uint16_t value);
static inline void printComments(uint16_t value);

