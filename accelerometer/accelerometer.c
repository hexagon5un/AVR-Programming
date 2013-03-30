/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/sleep.h>


#include "pinDefines.h"
#include "macros.h"

#define PADDING               5
#define SENSITIVITY_SAMPLES  20
#define NUM_BLINKS           20   
#define ON_TICKS             500	/* in hundredths of seconds */

// -------- Global Variables --------- //    
volatile uint16_t ticks;

// -------- Functions --------- //
static inline void initTicks(){
  TCCR0A |= (1 << WGM01);	/* CTC mode */
  TCCR0B |= (1<<CS00)|(1<<CS02);	/* 1 MHz / 1024 */
  TIMSK |= (1<<OCIE0A); 	/* output compare interrupt enable*/
  OCR0A = 10;			/* 1 Mhz / 1024 / 100 = 102.4 ms */
  sei();			/* set (global) enable interrupt bit */
  ticks=0;
}

ISR(TIMER0_COMPA_vect){
  ticks++;
}


int main(void){

  // -------- Inits --------- //
  uint8_t i;
  uint16_t lightsOutTime;
  uint16_t adcValue;
  uint16_t maxValue;
  uint16_t minValue;
  // Initializations here
  /* 2 LEDs as output, pos and neg excursions */
  LED_DDR |= (1 << LED1) | (1<< LED2);
  LED_PORT |= (1<<LED1) | (1<<LED2); /* Both off */
  SWITCH_DDR |= (1 << SWITCH);	     /* turns light on */

  /* Setup system timing and wait a bit */
  initTicks();
  ticks = 0;		       
  while(ticks < ON_TICKS){;}

  /* Init ADC */
  ADMUX = 1;			/*  ADC1 */
  ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1); /* prescale 8, start up */
  DIDR0 |= _BV(ADC1D);					     /* disable digital input for power saving */

  /* Auto-sensitivity routine */
  maxValue = 0;			/* start with small max, big min */
  minValue = 1023;
  LED_PORT ^= (1 << LED1);
  for (i = 0; i < SENSITIVITY_SAMPLES; i++){
    /* blink to let you know it's happening */
    LED_PORT ^= (1 << LED1);
    LED_PORT ^= (1 << LED2);
    _delay_ms(200);
    /* sample */
    ADCSRA |= _BV(ADSC);		
    loop_until_bit_is_clear(ADCSRA, ADSC);
    adcValue = ADC;
    /* update max/min */
    if (adcValue > maxValue){
      maxValue = adcValue;
    }
    else if (adcValue < minValue){
      minValue = adcValue;
    }
  }
  /* pad values to allow for deadband */
  maxValue += PADDING;
  minValue -= PADDING;
  
  /* End-of-calibration lightshow */
  LED_PORT |= (1<<LED1) | (1<<LED2); /* Both off */
  for (i=0; i < 8; i++){
    LED_PORT ^= (1 << LED1);
    LED_PORT ^= (1 << LED2);
    _delay_ms(100);
  }

  // ------ Event loop ------ //
  while(1){     

    /* Do conversion */
    ADCSRA |= _BV(ADSC);		
    loop_until_bit_is_clear(ADCSRA, ADSC);
    adcValue = ADC;
    
    /* Light up if outside threshold */
    if (adcValue < minValue){
      LED_PORT &= ~(1 << LED1);	/* one on */
      SWITCH_PORT |= (1 << SWITCH); /* turn switch on */
      lightsOutTime = ticks + ON_TICKS;
    }
    else if (adcValue > maxValue){
      LED_PORT &= ~(1 << LED2); /* two on */
      SWITCH_PORT |= (1 << SWITCH); /* turn switch on */
      lightsOutTime = ticks + ON_TICKS;
    }
    else{
      /* Nothing seen, turn off light when it's time */
      LED_PORT |= (1<<LED1) | (1<<LED2); /* Both off */
      if (ticks == lightsOutTime){
	SWITCH_PORT &= ~(1 << SWITCH); /* turn switch off */
	_delay_ms(100);		       /* there's some glitch when power back on */
      }
    }

    
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

