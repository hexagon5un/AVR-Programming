

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "pinDefines.h"
#include "USART.h"

#define CALIBRATION_SAMPLES  50 /* number samples in calibration phase */
#define ON_TICKS             200             /* 2 sec, in 1/100 second */

#define USE_SENSITIVITY_POT  0    /* set to 1 if you have pot attached */
#define SENSITIVITY_ADC      PC5               /* sensitivity pot here */

// -------- Global Variables --------- //
volatile uint16_t ticks;                      /* for system tick clock */

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

void initTicks(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0B |= (1 << CS00) | (1 << CS02);                 /* 8 MHz / 1024 */
  TIMSK0 |= (1 << OCIE0A);          /* output compare interrupt enable */
  OCR0A = 77;                        /* 78 / (8MHz / 1024) =  9.984 ms */
  sei();                          /* set (global) enable interrupt bit */
  ticks = 0;
}

ISR(TIMER0_COMPA_vect) {
  ticks++;
}

void sleepDelay(uint16_t numTicks) {
  numTicks += ticks;                    /* when should I wake back up? */
  while (!(ticks == numTicks)) {
    sleep_mode();
  }
}

int main(void) {

  // -------- Inits --------- //
  uint16_t lightsOutTime=100;                      /* timer for the switch */
  uint16_t adcValue;

	uint16_t maxValue=0;                      /* max seen during calibration */
	uint16_t minValue=1023;                      /* min seen during calibration */
	uint16_t middleValue=511; 
  // Initializations here
                                 /* 2 LEDs as output, "switch" on LED7 */
  LED_DDR = ((1 << LED0) | (1 << LED1) | (1 << LED7));
  initADC();
  initUSART();
  initTicks();
  set_sleep_mode(SLEEP_MODE_IDLE);

  // ------ Event loop ------ //
  while (1) {
		adcValue = readADC(PIEZO);                        /* Do conversion */
		// transmitByte((adcValue - 127));        /* quasi-seismograph output */
									            /* Light up display if outside threshold */


		middleValue = ((adcValue + 31*middleValue + 16) >> 5) ;
		if (adcValue > middleValue){
			maxValue = ((adcValue + 15*maxValue + 8) >> 4) ;
		}
		else if (adcValue < middleValue){
			minValue = ((adcValue + 15*minValue + 8) >> 4) ;
		}
		else { /* value in the middle, shrink extremes down to the middle */ 
			maxValue = ((middleValue + 31*maxValue + 16) >> 5) ;
			minValue = ((middleValue + 31*minValue + 16) >> 5) ;
		}
		transmitByte(adcValue >> 2);
		/*transmitByte(minValue >> 2);*/
		/*transmitByte(maxValue >> 2);*/
		/*sleepDelay(1);*/

		if (adcValue < minValue) {
      LED_PORT = (1 << LED0) | (1 << LED7);         /* one LED, switch */
      lightsOutTime = ticks + ON_TICKS;     /* leave light on until... */
    }
    else if (adcValue > maxValue) {
      LED_PORT = (1 << LED1) | (1 << LED7);       /* other LED, switch */
      lightsOutTime = ticks + ON_TICKS;     /* leave light on until... */
    }
    else {                            /* Nothing seen, turn off lights */
      LED_PORT &= ~(1 << LED0);
      LED_PORT &= ~(1 << LED1);                            /* Both off */
      if (ticks == lightsOutTime) {  /* if no activity in given period */
        LED_PORT &= ~(1 << LED7);                   /* turn switch off */
        sleepDelay(10);          /* delay in case of switch transients */
      }
    }
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
