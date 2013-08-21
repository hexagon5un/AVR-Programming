/* ADC Voltmeter
 * Continuously outputs voltage over the serial line.
 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>                           /* for ADC sleep mode */
#include <math.h>                           /* for round() and floor() */

#include "pinDefines.h"
#include "USART.h"

/* Note: This voltmeter is only as accurate as your reference voltage.
 * If you want four digits of accuracy, need to measure your AVCC well.
 * Measure either AVCC of the voltage on AREF and enter it here.
*/
#define REF_VCC 5.053
                               /* measured division by voltage divider */
#define VOLTAGE_DIV_FACTOR  3.114


// -------- Functions --------- //
void initADC(void) {
  ADMUX |= (0b00001111 & PC5);                      /* set mux to ADC5 */
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2);    /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
}

void setupADCSleepmode(void) {
  set_sleep_mode(SLEEP_MODE_ADC);            /* defined in avr/sleep.h */
  ADCSRA |= (1 << ADIE);                       /* enable ADC interrupt */
  sei();                                   /* enable global interrupts */
}

EMPTY_INTERRUPT(ADC_vect);

uint16_t oversample16x(void) {
  uint16_t oversampledValue = 0;
  uint8_t i;
  for (i = 0; i < 16; i++) {
    sleep_mode();                   /* chip to sleep, takes ADC sample */
    oversampledValue += ADC;                        /* add them up 16x */
  }
  return (oversampledValue >> 2);          /* divide back down by four */
}

void printFloat(float number) {
  number = round(number * 100) / 100; /* round off to 2 decimal places */
  transmitByte('0' + number / 10);                       /* tens place */
  transmitByte('0' + number - 10 * floor(number / 10));        /* ones */
  transmitByte('.');
  transmitByte('0' + (number * 10) - floor(number) * 10);    /* tenths */
                                                   /* hundredths place */
  transmitByte('0' + (number * 100) - floor(number * 10) * 10);
  printString("\r\n");
}

int main(void) {

  float voltage;

  // -------- Inits --------- //
  initUSART();
  printString("\r\nDigital Voltmeter\r\n\r\n");
  initADC();
  setupADCSleepmode();

  // ------ Event loop ------ //

  while (1) {

    voltage = oversample16x() * VOLTAGE_DIV_FACTOR * REF_VCC / 4096;
    printFloat(voltage);
    /*  alternatively, just print it out:
     *  printWord(voltage*100);
     *  but then you have to remember the decimal place
     */
    _delay_ms(500);

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
