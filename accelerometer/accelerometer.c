/*  */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "pinDefines.h"
#include "macros.h"
#include "UART.h"

#define CALIBRATION_SAMPLES  20	 /* number samples in calibration phase */
#define ON_TICKS             250 /* 2.5 sec, in 1/100 second */

// -------- Global Variables --------- //    
volatile uint16_t ticks;	/* for system tick clock */

// -------- Functions --------- //
static inline void initTicks(){
  TCCR0A |= (1 << WGM01);	  /* CTC mode */
  TCCR0B |= (1<<CS00) | (1<<CS02);/* 8 MHz / 1024 */
  TIMSK0 |= (1<<OCIE0A); 	  /* output compare interrupt enable*/
  OCR0A = 77;			  /* 78 / (8MHz / 1024) =  9.984 ms*/
  sei();			  /* set (global) enable interrupt bit */
  ticks=0;
}

ISR(TIMER0_COMPA_vect){
  ticks++;
}

static inline void initADC(void){
  ADMUX  |= (0b00001111 & PIEZO);  /* set mux to ADC2 */
  ADMUX  |= (1 << REFS0);        /* reference voltage to AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);        /* enable ADC */
}

static inline void sleepDelay(uint16_t numTicks){
  numTicks += ticks;  	   /* when should I wake back up? */
  while(ticks < numTicks){
    sleep_mode();
  }
}

static inline void blink(uint8_t times){
  /* Simple loop toggles LEDs on # times */
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
  uint16_t padding;

  // Initializations here
  set_sleep_mode(SLEEP_MODE_IDLE);
  
  /* 2 LEDs as output, "switch" on LED7 */
  LED_DDR = ((1<<LED0) | (1<<LED1) | (1<<LED7));

  initADC();
  initUART();

  /* Setup system timing and wait a bit */
  initTicks();
  sleepDelay(100); 		/* let all settle for 1 sec after reset */

  /* Sensitivity calibration routine */
  maxValue = 0;			/* start with small max, big min */
  minValue = 1023;
  set_bit(LED_PORT, LED0);	/* set one LED on initially */
  for (i = 0; i < CALIBRATION_SAMPLES; i++){
    /* blink alternately while calibrating */
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
  padding = maxValue - minValue;
  transmitByte((minValue-127)); /* lower eight bits, centered */
  transmitByte((maxValue-127)); 
  /* Blink both to signal done */
  LED_PORT = 0;
  blink(20);

  // ------ Event loop ------ //
  while(1){     

    /* Do conversion */
    set_bit(ADCSRA, ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    adcValue = ADC;
    transmitByte((adcValue-127)); /* quasi-seismograph output */

    /* Light up if outside threshold */
    if (adcValue < (minValue - padding)){
      LED_PORT = (1 << LED0) | (1 << LED7); /* one LED, switch */
      lightsOutTime = ticks + ON_TICKS;
    }
    else if (adcValue > (maxValue + padding)){
      LED_PORT = (1 << LED1) | (1 << LED7); /* other LED, switch */
      lightsOutTime = ticks + ON_TICKS;
    }
    else{ /* Nothing seen, turn off light when it's time */
      clear_bit(LED_PORT, LED0);
      clear_bit(LED_PORT, LED1);   /* Both off */
      if (ticks == lightsOutTime){
	clear_bit(LED_PORT, LED7); /* turn switch off */
	sleepDelay(10);		   /* delay in case of switch transients */
      }
    }

    sleep_mode();		/* sleep for the rest of the system tick */

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

