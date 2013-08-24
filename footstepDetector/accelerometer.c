

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
uint16_t maxValue;                      /* max seen during calibration */
uint16_t minValue;                      /* min seen during calibration */

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

void calibratePiezo(void) {
  uint8_t i;
  uint16_t adc;
  maxValue = 0;                       /* start with small max, big min */
  minValue = 1023;

  sleepDelay(200);             /* let all settle for 2 sec after reset */

  for (i = 0; i < CALIBRATION_SAMPLES; i++) {
    adc = readADC(PIEZO);                               /* sample once */
    if (adc > maxValue) {                            /* update max/min */
      maxValue = adc;
    }
    else if (adc < minValue) {
      minValue = adc;
    }
                                            /* blink while calibrating */
    LED_PORT ^= ((1 << LED0) | (1 << LED1));
    sleepDelay(10);
  }
  LED_PORT = 0;                           /* all off, done calibrating */
                                        /* send min, max for debugging */
  transmitByte((minValue - 127));
  transmitByte((maxValue - 127));
  sleepDelay(200);
}
int main(void) {

  // -------- Inits --------- //
  uint8_t i;
  uint16_t lightsOutTime;                      /* timer for the switch */
  uint16_t adcValue;
  uint16_t padding;                   /* makes deadband around max/min */

  // Initializations here
                                 /* 2 LEDs as output, "switch" on LED7 */
  LED_DDR = ((1 << LED0) | (1 << LED1) | (1 << LED7));
  initADC();
  initUSART();
  initTicks();
  set_sleep_mode(SLEEP_MODE_IDLE);
  calibratePiezo();
                               /* more background noise = more padding */
  padding = maxValue - minValue;

  // ------ Event loop ------ //
  while (1) {
		adcValue = readADC(PIEZO);                        /* Do conversion */
    transmitByte((adcValue - 127));        /* quasi-seismograph output */
                              /* Light up display if outside threshold */
    if (adcValue < (minValue - padding)) {
      LED_PORT = (1 << LED0) | (1 << LED7);         /* one LED, switch */
      lightsOutTime = ticks + ON_TICKS;     /* leave light on until... */
    }
    else if (adcValue > (maxValue + padding)) {
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

      /* optionally allow user to change padding/sensitivity with knob */
#if USE_SENSITIVITY_POT
    padding = readADC(SENSITIVITY_ADC);   /* read sensitivity from pot */
    padding = (padding >> 4);            /* scale down to useful range */
#endif

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
