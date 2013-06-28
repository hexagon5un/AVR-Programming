/*  */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include "pinDefines.h"
#include "USART.h"

#define CALIBRATION_SAMPLES  20	 /* number samples in calibration phase */
#define USE_SENSITIVITY_POT  0   /* set to 1 if you have pot attached */
#define SENSITIVITY_ADC      PC5 /* sensitivity pot here */


// -------- Functions --------- //
void initADC(void){
  ADMUX  |= (1 << REFS0);        /* reference voltage to AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);        /* enable ADC */
}

uint16_t readADC(uint8_t channel){
  ADMUX = (0b11110000 & ADMUX) | channel;
  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return(ADC);
}

int main(void){

  // -------- Inits --------- //
  uint8_t i;
  uint16_t adcValue;		/* current value on ADC */
  uint16_t maxValue;		/* max seen during calibration */
  uint16_t minValue;		/* min seen during calibration */
  uint16_t padding;		/* makes deadband around max/min */

  // Initializations here
  LED_DDR = ((1<<LED0) | (1<<LED1)); * 2 LEDs as output */
  initADC();
  initUSART();
  _delay_ms(2000); 		/* let all settle for 2 sec after reset */

  /* Sensitivity calibration routine */
  maxValue = 0;			/* start with small max, big min */
  minValue = 1023;
  for (i = 0; i < CALIBRATION_SAMPLES; i++){
    adcValue = readADC(PIEZO); 	/* sample once */
    if (adcValue > maxValue){   /* update max/min */
      maxValue = adcValue;
    }
    else if (adcValue < minValue){
      minValue = adcValue;
    }
    /* blink while calibrating */	
    LED_PORT ^= ((1<<LED0)|(1<<LED1));
    _delay_ms(200);
  }
  LED_PORT = 0;			/* all off, done calibrating */

  /* more background noise = more padding */
  padding = maxValue - minValue; 
  /* send min, max for debugging */
  transmitByte((minValue-127)); 
  transmitByte((maxValue-127)); 
  _delay_ms(2000);

  // ------ Event loop ------ //
  while(1){     

    /* Do conversion */
    adcValue = readADC(PIEZO);
    transmitByte((adcValue-127)); /* quasi-seismograph output */

/* optionally allow user to change padding/sensitivity with knob */
#if USE_SENSITIVITY_POT 
    padding = readADC(SENSITIVITY_ADC);	/* read sensitivity from pot */
    padding = (padding >> 4);	/* optionally scale to useful range */
#endif    

    /* Light up display if outside threshold */
    if (adcValue < (minValue - padding)){
      LED_PORT = (1 << LED0); /* one LED */
    }
    else if (adcValue > (maxValue + padding)){
      LED_PORT = (1 << LED1); /* other LED */
    }
    else{ /* Nothing seen, turn off lights */
      LED_PORT &= ~(1 << LED0);
      LED_PORT &= ~(1 << LED1);   /* Both off */
    }
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

