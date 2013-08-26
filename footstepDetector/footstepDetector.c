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
#define CYCLE_DELAY           5                        /* milliseconds */
#define INITIAL_PADDING       0

#define SWITCH              PB7

#define USE_POT               0  /* define to 1 if using potentiometer */

#if USE_POT
  #define POT               PC5     /* optional sensitivity pot on PC5 */
#endif

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

int main(void) {
  // -------- Inits --------- //
  uint16_t lightsOutTime = 0;                  /* timer for the switch */
  uint16_t adcValue;
	uint16_t middleValue = 511;                         /* average value */
	uint16_t highValue = 1023;                         /* average value */
	uint16_t lowValue = 0;                        /* average value */
	uint8_t padding = INITIAL_PADDING;
                          /* 2 LEDs as output, "switch" on SWITCH */
  LED_DDR = ((1 << LED0) | (1 << LED1) | (1 << SWITCH));
  initADC();
  initUSART();

	/*uint16_t avgArray[64];*/
	/*uint8_t i, j;*/
	/*for (i=0; i<64; i++){*/
		/*avgArray[i]=0;*/
	/*}*/
	/*uint16_t runningSum=0;*/
	// ------ Event loop ------ //
  while (1) {
 		adcValue = 4*readADC(PIEZO);

		/*i++;*/
		/*i &= 0b00111111;*/
		/*runningSum -= avgArray[i];*/
		/*avgArray[i] = adcValue;*/
		/*runningSum += avgArray[i];*/
		/*middleValue = runningSum >> 6;*/
//		adcValue = readADC(PIEZO);
    // Keep long-running moving average -- will average out to midpoint
		middleValue = ((adcValue + 15 * middleValue +  8) >> 4);
		if (adcValue > (middleValue+padding)){
			highValue = ((adcValue + 15 * highValue + 8) >> 4);
		}
		if (adcValue < (middleValue-padding)){
			lowValue = ((adcValue + 15 * lowValue + 8) >> 4);
		}
		/*if( (middleValue-10)<adcValue && adcValue < (middleValue+10) ){*/
			/*transmitByte(0);*/
			/*highValue = ((adcValue + 15 * highValue + 8) >> 4);*/
			/*lowValue = ((adcValue + 15 * lowValue + 8) >> 4);*/
		/*}*/
		/*lowValue = 1023;*/
		/*highValue = 0;*/
		/*for (j=0; j<64; j++){*/
			/*if (avgArray[j] > highValue){*/
				/*highValue = avgArray[j];*/
			/*}	*/
			/*if (avgArray[j] < lowValue){*/
				/*lowValue = avgArray[j];*/
			/*}	*/
		/*}*/
		/*padding = highValue - lowValue;*/
		/*padding = 10;*/
    // Now check to see if ADC value above or below thresholds
    if (adcValue < 2*lowValue-middleValue) {
      LED_PORT = (1 << LED0) | (1 << SWITCH);       /* one LED, switch */
      lightsOutTime = ON_TIME / CYCLE_DELAY;            /* reset timer */
    }
    else if (adcValue > 2*highValue-middleValue) {
      LED_PORT = (1 << LED1) | (1 << SWITCH);     /* other LED, switch */
      lightsOutTime = ON_TIME / CYCLE_DELAY;            /* reset timer */
    }
    else {                            /* Nothing seen, turn off lights */
      LED_PORT &= ~(1 << LED0);
      LED_PORT &= ~(1 << LED1);                            /* Both off */
      if (lightsOutTime > 0) {                 /* still have time left */
        lightsOutTime--;
      }
      else {                                              /* time's up */
        LED_PORT &= ~(1 << SWITCH);                 /* turn switch off */
      }
    }
#if USE_POT                      /* optional sensitivity potentiometer */
    padding = readADC(POT) >> 4;         /* scale down to useful range */
#endif
  
		// Serial output and delay
		/*if (i==0){*/
			transmitByte((adcValue>>2) -512+127);
			/*transmitByte((middleValue >> 2) -512+127);*/
			/*transmitByte((highValue - lowValue) >> 2);*/
			transmitByte((lowValue >> 2)-512+127);
			transmitByte((highValue >> 2)-512+127);
		/*}*/
		 _delay_ms(CYCLE_DELAY);
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
