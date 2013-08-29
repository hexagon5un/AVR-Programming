/*
 *      Sensitive footstep-detector.
 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "pinDefines.h"
#include "USART.h"

#define ON_TIME            2000                        /* milliseconds */
#define CYCLE_DELAY          10                        /* milliseconds */
#define INITIAL_SENSITIVITY  10

#define SWITCH              PB7

#define USE_POT           0  /* define to 1 if using potentiometer */
#if USE_POT
  #define POT           PC5     /* optional sensitivity pot on PC5 */
#endif

// -------- Functions --------- //
void initADC(void) {
  ADMUX |= (1 << REFS0);                  /* reference voltage to AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2);    /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
}

uint16_t readADC(uint8_t channel) {
  ADMUX = (0b11110000 & ADMUX) | channel;
  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return (ADC);
}

int main(void) {
  // -------- Inits --------- //
  uint16_t lightsOutTimer = 0;                 /* timer for the switch */
  uint16_t adcValue;
  uint16_t middleValue = 511;
  uint16_t highValue = 1023;
  uint16_t lowValue = 0;
	uint16_t noiseVolume = 0;
	uint8_t sensitivity = INITIAL_SENSITIVITY;
                               /* 2 LEDs as output, "switch" on SWITCH */
  LED_DDR = ((1 << LED0) | (1 << LED1) | (1 << SWITCH));
  initADC();
  initUSART();

  // ------ Event loop ------ //
  while (1) {
    adcValue = readADC(PIEZO);

   /* Keep long-running moving average -- will average out to midpoint */
		middleValue = adcValue + middleValue - ((middleValue - 8) >> 4) ; 
		// The "sensitivity" variable keeps the high and low bounds away
    // from the middle value, which desensitizes the sensor.
    if (adcValue > ((middleValue >> 4) + sensitivity)) {
      highValue = adcValue + highValue - ((highValue - 8) >> 4) ;
    }
    if (adcValue < ((middleValue >> 4) - sensitivity)) {
      lowValue = adcValue + lowValue - ((lowValue - 8) >> 4) ;
    }
		noiseVolume = highValue - lowValue;

            /* Now check to see if ADC value above or below thresholds */
	 /* Comparison with << 4 b/c the other values come from EWMA */ 
		if ( ( adcValue<<4) < (middleValue - noiseVolume)) {
      LED_PORT = (1 << LED0) | (1 << SWITCH);       /* one LED, switch */
      lightsOutTimer = ON_TIME / CYCLE_DELAY;           /* reset timer */
    }
    else if ((adcValue<<4) > (middleValue + noiseVolume)) {
      LED_PORT = (1 << LED1) | (1 << SWITCH);     /* other LED, switch */
      lightsOutTimer = ON_TIME / CYCLE_DELAY;           /* reset timer */
    }
    else {                            /* Nothing seen, turn off lights */
      LED_PORT &= ~(1 << LED0);
      LED_PORT &= ~(1 << LED1);                            /* Both off */
      if (lightsOutTimer > 0) {                  /* time left on timer */
        lightsOutTimer--;
      }
      else {                                              /* time's up */
        LED_PORT &= ~(1 << SWITCH);                 /* turn switch off */
      }
    }
#if USE_POT                      /* optional sensitivity potentiometer */
    sensitivity = readADC(POT) >> 4;     /* scale down to useful range */
#endif

    // Serial output and delay
		/* ADC is 10-bits, here recenter around 127  */ 
		transmitByte(adcValue  - 512 + 127);
		/*transmitByte((adcValue << 4) - middleValue);*/
		/*transmitByte(noiseVolume >> 4);*/
		/*transmitByte((middleValue >> 4) - 512 + 127);*/
		/*transmitByte((lowValue >> 4) - 512 + 127);*/
		/*transmitByte((highValue >> 4) - 512 + 127);*/
    _delay_ms(CYCLE_DELAY);
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
