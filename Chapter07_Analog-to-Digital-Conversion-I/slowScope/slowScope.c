// Slow-scope.  A free-running AVR / ADC "oscilloscope"

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"

#define SAMPLE_DELAY  20 /* ms, controls the scroll-speed of the scope */

// -------- Functions --------- //
static inline void initFreerunningADC(void) {
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    /* ADC clock prescaler /32 */

  ADMUX |= (1 << ADLAR);     /* left-adjust result, return only 8 bits */

  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
  ADCSRA |= (1 << ADATE);                       /* auto-trigger enable */
  ADCSRA |= (1 << ADSC);                     /* start first conversion */
}

int main(void) {
  // -------- Inits --------- //
  initUSART();
  initFreerunningADC();
  // ------ Event loop ------ //
  while (1) {
    transmitByte(ADCH);       /* transmit the high byte, left-adjusted */
    _delay_ms(SAMPLE_DELAY);
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
