/* 
   Capacitive touch sensor demo

 */

#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>         
#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define PRESS_DIFFERENCE   -20	/* number of counts difference to trigger
				decrease to make more sensitive. 
				increase if getting false hits */
#define RELEASE_DIFFERENCE  20	/* number of counts difference to trigger
				decrease to make more sensitive. 
				increase if getting false hits */

/* Global Variables */

volatile uint16_t chargeCycleCount;
volatile uint16_t baselineCycleCount;
volatile int16_t  cycleDifference;
volatile uint8_t  touched;

/* PCINT configured to charge and discharge the capacitor continuously */
ISR(PCINT1_vect){
  chargeCycleCount++;		        /* count this change */
  
  set_bit(CAP_SENSOR_PORT, CAP_SENSOR); /* set high to charge capacitor */
  set_bit(CAP_SENSOR_DDR, CAP_SENSOR);  /* output mode */
  _delay_us(1);			        /* charging delay */
    
  clear_bit(CAP_SENSOR_DDR, CAP_SENSOR);   /* set as input */
  clear_bit(CAP_SENSOR_PORT, CAP_SENSOR);  /* with no pullup */
  set_bit(PCIFR, PCIF1);		   /* clear the pin-change interrupt */
}

  
/* This interrupt runs on regular schedule, decides if pressed */
ISR(TIMER0_COMPA_vect){

  /* Moving-average baseline value, auto-adjusts to environment */
  baselineCycleCount = (15*baselineCycleCount + chargeCycleCount) >> 4; 
  cycleDifference = chargeCycleCount - baselineCycleCount;

  if (cycleDifference < PRESS_DIFFERENCE){
    touched = 1;  
  }
  if (cycleDifference > RELEASE_DIFFERENCE){
    touched = 0;  
  }  

  /* Reset */
  chargeCycleCount = 0;
  /* Charge capacitor up at least once per period, 
     just to make sure still cycling */
  set_bit(CAP_SENSOR_DDR, CAP_SENSOR);
  set_bit(CAP_SENSOR_PORT, CAP_SENSOR);
}

static inline void initTimer0(void){
  set_bit(TCCR0A, WGM01);       /* CTC mode */
  set_bit(TCCR0B, CS02);        
  set_bit(TCCR0B, CS00);        /* 8 MHz / 1024 clock */
  set_bit(TIMSK0, OCIE0A);      /* output compare interrupt enable*/
  OCR0A = 200;                  /* roughly 25 ms */ 
  sei();                        /* set enable interrupt bit */
}

static inline void initPinChangeInterrupt(void){
  /* B, C, D = 0, 1, 2 */
  set_bit(PCICR, PCIE1);   /* enable Pin-change interrupts 1 (bank C) */
  set_bit(PCMSK1, PCINT9); /* enable specific interrupt for our pin PC1 */
  sei();		       /* set (global) interrupt enable bit */
}


int main(void){
  
  // -------- Inits --------- //
  initUART();
  LED_DDR = 0xff;
  initTimer0();
  initPinChangeInterrupt();

  // ------ Event loop ------ //
  while(1){			
    
    if (touched){
      LED_PORT = 0xff;
    }
    else{
      LED_PORT = 0;
    }
    
    /* Uncomment one or more of these for debugging... */
    // transmitByte(chargeCycleCount >> 4);
    // transmitByte(baselineCycleCount >> 4);
    // transmitByte(((cycleDifference) >> 2) + 128);

    // transmitByte((((cycleDifference) >> 2) + 128) >> 5);
    // LED_PORT = BV(((((cycleDifference) >> 2) + 128) >> 5));

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}
