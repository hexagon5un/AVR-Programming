// Quick and dirty adjustable-threshold night-light.

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"

uint16_t readADC(uint8_t channel) {
  ADMUX = (0xf0 & ADMUX) | channel;
  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return (ADC);
}

int main(void) {
  // -------- Inits --------- //
  uint16_t lightThreshold;
  uint16_t sensorValue;
  // Set up ADC
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);     /* ADC clock prescaler /8 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */

  LED_DDR = 0xff;
  // ------ Event loop ------ //
  while (1) {

    lightThreshold = readADC(POT);
    sensorValue = readADC(LIGHT_SENSOR);

    if (sensorValue < lightThreshold) {
      LED_PORT = 0xff;
    }
    else {
      LED_PORT = 0x00;
    }
  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}
