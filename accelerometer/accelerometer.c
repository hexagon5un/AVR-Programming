/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "pinDefines.h"
#include "macros.h"

#define PADDING              2
#define CALIBRATION_SAMPLES  20
#define ON_TICKS             250	/* 2.5 sec, in hundredths of seconds */

// -------- Global Variables --------- //    
volatile uint16_t ticks;

// -------- Functions --------- //
static inline void initTicks(){
  TCCR0A |= (1 << WGM01);	/* CTC mode */
  TCCR0B |= (1<<CS00)|(1<<CS02);/* 8 MHz / 1024 */
  TIMSK0 |= (1<<OCIE0A); 	/* output compare interrupt enable*/
  OCR0A = 77;			/* 78 / (8MHz / 1024) =  9.984 ms*/
  sei();			/* set (global) enable interrupt bit */
  ticks=0;
}

ISR(TIMER0_COMPA_vect){
  ticks++;
}

static inline void initADC(void){
  ADMUX  |= (0b00001111 & PC1);  /* set mux to ADC1 */
  ADMUX  |= (1 << REFS0);        /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);        /* enable ADC */
  DIDR0 |= _BV(ADC1D);	  /* disable digital input for power saving */
}

static inline void sleepDelay(uint16_t numTicks){
  ticks = 0;	  
  while(ticks < numTicks){
    sleep_mode();
  }
}

static inline void blink(uint8_t times){
  while(times){
    toggle_bit(LED_PORT, LED0);
    toggle_bit(LED_PORT, LED1);
    sleepDelay(20);
    times--;
  }
}

int main(void){

  // -------- Inits --------- //
  uint8_t i;
  uint16_t lightsOutTime;
  uint16_t adcValue;
  uint16_t maxValue;
  uint16_t minValue;
  
  // Initializations here
  set_sleep_mode(SLEEP_MODE_IDLE);
  
  /* 2 LEDs as output, pos and neg excursions */
  set_bit(LED_DDR, LED0);
  set_bit(LED_DDR, LED1);
  set_bit(LED_DDR, LED7);	     /* active switch */

  initADC();

  /* Setup system timing and wait a bit */
  initTicks();
  sleepDelay(100); 		/* let all settle for 1 sec after reset */
  set_bit(LED_PORT, LED0);

  /* Auto-sensitivity calibration routine */
  maxValue = 0;			/* start with small max, big min */
  minValue = 1023;
  for (i = 0; i < CALIBRATION_SAMPLES; i++){
    /* blink alternately = calibrating */
    blink(1);
    /* sample */
    set_bit(ADCSRA, ADSC);		
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

  /* Blink both to signal done */
  clear_bit(LED_PORT, LED0);
  clear_bit(LED_PORT, LED1);
  blink(10);

  // ------ Event loop ------ //
  while(1){     

    /* Do conversion */
    set_bit(ADCSRA, ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    adcValue = ADC;
    
    /* Light up if outside threshold */
    if (adcValue < (minValue-PADDING)){
      set_bit(LED_PORT, LED0);	/* one on */
      set_bit(LED_PORT, LED7);  /* turn switch on */
      lightsOutTime = ticks + ON_TICKS;
    }
    else if (adcValue > (maxValue+PADDING)){
      set_bit(LED_PORT, LED1); /* two on */
      set_bit(LED_PORT, LED7); /* turn switch on */
      lightsOutTime = ticks + ON_TICKS;
    }
    else{
      /* Nothing seen, turn off light when it's time */
      clear_bit(LED_PORT, LED0);
      clear_bit(LED_PORT, LED1); /* Both off */
      if (ticks == lightsOutTime){
	clear_bit(LED_PORT, LED7); /* turn switch off */
      }
    }

    sleep_mode();		/* sleep for the rest of the tick */

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

