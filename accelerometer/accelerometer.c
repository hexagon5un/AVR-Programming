/*
 * Maximally-sensitive footstep-detector.  
 * Uses moving averages to automatically separate signal from noise
 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "pinDefines.h"
#include "USART.h"

#define ON_TIME             2000             /* milliseconds */
#define CYCLE_DELAY           10             /* milliseconds */

#define SWITCH              PB7 /* LED or light-switch.  It's all in the hookup. */ 
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
  uint16_t lightsOutTime=0;                      /* timer for the switch */
	uint16_t adcValue;
	/*uint16_t maxValue=0;                      [> upper activity threshold <]*/
	/*uint16_t minValue=1023;                      [> lower activity threshold <]*/
	uint16_t middleValue=511;  /* average value */ 
	uint16_t padding = 6;

	/* 2 LEDs as output, "switch" on SWITCH */
  LED_DDR = ((1 << LED0) | (1 << LED1) | (1 << SWITCH));
  initADC();
  initUSART();

  // ------ Event loop ------ //
  while (1) {
		adcValue = readADC(PIEZO);                        
    // Keep long-running moving average -- should approximate midpoint  
		middleValue = ((adcValue + 63*middleValue + 32) >> 6);
		
		// Update the thresholds  
		if (adcValue > middleValue){
			/*maxValue = ((adcValue + 63*maxValue + 32) >> 6);*/
			padding = ((adcValue - middleValue) +  3*padding + 2) >> 2;
		}
		else if (adcValue < middleValue){
			/*minValue = ((adcValue + 63*minValue + 32) >> 6);*/
			padding = ((middleValue - adcValue) +  3*padding + 2) >> 2;
		}
	 /*else{	*/
			/*maxValue = ((adcValue + 31*maxValue + 15) >> 5) ;*/
			/*minValue = ((adcValue + 31*minValue + 15) >> 5) ;*/
		/*}*/
		// Now check to see if ADC value above or below thresholds
		if (adcValue < (middleValue - 2*padding)) {
      LED_PORT = (1 << LED0) | (1 << SWITCH);         /* one LED, switch */
      lightsOutTime = ON_TIME/CYCLE_DELAY;          /* reset timer */
    }
    else if (adcValue > (middleValue + 2*padding)) {
      LED_PORT = (1 << LED1) | (1 << SWITCH);       /* other LED, switch */
      lightsOutTime = ON_TIME/CYCLE_DELAY;     /* reset timer */
    }
    else {                            /* Nothing seen, turn off lights */
      LED_PORT &= ~(1 << LED0);
      LED_PORT &= ~(1 << LED1);                            /* Both off */
			/*maxValue = ((adcValue + 31*maxValue + 15) >> 5) ;*/
			/*minValue = ((adcValue + 31*minValue + 15) >> 5) ;*/
      if (lightsOutTime > 0) {  /* if no activity */
				lightsOutTime--; /* And decrement lights-out timer */ 
      }
			else{
        LED_PORT &= ~(1 << SWITCH);                   /* turn switch off */
			}
    }

		// Serial output and delay
		/*transmitByte(adcValue >> 2);*/
		/*transmitByte(minValue >> 2); */
		/*transmitByte(maxValue >> 2);*/
		transmitByte(padding);
		_delay_ms(CYCLE_DELAY);
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
