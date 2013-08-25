/*
 * Maximally-sensitive footstep-detector.  
 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "pinDefines.h"
#include "USART.h"

#define ON_TIME            2000             /* milliseconds */
#define CYCLE_DELAY           9             /* milliseconds */
#define INITIAL_PADDING      10

#define SWITCH              PB7 /* LED or light-switch.  It's all in the hookup. */ 

#define USE_POT               0 /* define to 1 if using potentiometer */ 
#if USE_POT
  #define POT               PC5  /* optional sensitivity pot on PC5 */ 
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
  uint16_t lightsOutTime=0;                      /* timer for the switch */
	uint16_t adcValue;
	uint16_t middleValue=511;  /* average value */ 
	uint8_t padding = INITIAL_PADDING;

	/* 2 LEDs as output, "switch" on SWITCH */
  LED_DDR = ((1 << LED0) | (1 << LED1) | (1 << SWITCH));
  initADC();
  initUSART();

  // ------ Event loop ------ //
  while (1) {
		adcValue = readADC(PIEZO);                        
    // Keep long-running moving average -- should approximate midpoint  
		middleValue = ((adcValue + 63*middleValue + 32) >> 6);
		
		// Now check to see if ADC value above or below thresholds
		if (adcValue < (middleValue - padding)) {
      LED_PORT = (1 << LED0) | (1 << SWITCH);         /* one LED, switch */
      lightsOutTime = ON_TIME/CYCLE_DELAY;          /* reset timer */
    }
    else if (adcValue > (middleValue + padding)) {
      LED_PORT = (1 << LED1) | (1 << SWITCH);       /* other LED, switch */
      lightsOutTime = ON_TIME/CYCLE_DELAY;     /* reset timer */
    }
    else {                            /* Nothing seen, turn off lights */
      LED_PORT &= ~(1 << LED0);
      LED_PORT &= ~(1 << LED1);                            /* Both off */
      if (lightsOutTime > 0) {  /* still have time left */
				lightsOutTime--;  
      }
			else{ /* time's up */ 
        LED_PORT &= ~(1 << SWITCH);                   /* turn switch off */
			}
    }
#if USE_POT  /* optional sensitivity potentiometer */ 
		padding = readADC(POT) >> 4;  /* scale down to useful range */ 
#endif

		// Serial output and delay
		transmitByte(adcValue >> 2);
		_delay_ms(CYCLE_DELAY);
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
