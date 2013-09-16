               /* Slow-scope.  A free-running AVR / ADC "oscilloscope" */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "USART.h"

// -------- Functions --------- //
static inline void initADC(void) {
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADMUX |= (1 << ADLAR);     /* left-adjust result, return only 8 bits */
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    /* ADC clock prescaler /32 */
  ADCSRA |= (1 << ADATE);                       /* auto-trigger enable */
  ADCSRA |= (1 << ADIE);             /* enable interrupt when complete */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
  ADCSRA |= (1 << ADSC);                     /* start first conversion */
  sei();
}

ISR(ADC_vect) {
  transmitByte(ADCH);    /* just transmit the high byte, left-adjusted */
}

int main(void) {
  // -------- Inits --------- //
  initUSART();
  initADC();
  // ------ Event loop ------ //
  while (1) {
    ;                      /* Nothing. It's all done in the interrupt. */
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
