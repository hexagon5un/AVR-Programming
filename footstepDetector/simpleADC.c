// ------- Preamble -------- //
#include <avr/io.h>

// -------- Functions --------- //
void initADC(void) {
  ADMUX  |= (1 << REFS0);                 /* reference voltage to AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2);    /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
}

uint16_t readADC(uint8_t channel) {
  ADMUX = (0b11110000 & ADMUX) | channel;
  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return (ADC);
}

