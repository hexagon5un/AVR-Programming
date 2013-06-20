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

volatile uint8_t  touched;	      /* Touch sensor state. */
volatile uint16_t baselineCycleCount; /* average number cycles */
volatile int16_t  cycleDifference;    /* difference between last count
				       and the average number */
volatile uint16_t chargeCycleCount;   /* internal ISR use only */

/* PCINT configured to charge and discharge the capacitor continuously */
ISR(PCINT1_vect){
  chargeCycleCount++;		        /* count this change */
  
  CAP_SENSOR_PORT |= (1 << CAP_SENSOR); /* set high to charge capacitor */
  CAP_SENSOR_DDR |= (1 << CAP_SENSOR);  /* output mode */
  _delay_us(1);			        /* charging delay */
    
  CAP_SENSOR_DDR &= ~(1 << CAP_SENSOR);   /* set as input */
  CAP_SENSOR_PORT &= ~(1 << CAP_SENSOR);  /* with no pullup */
  PCIFR |= (1 << PCIF1);		   /* clear the pin-change interrupt */
}

/* This interrupt runs on regular schedule, decides if pressed */
ISR(TIMER0_COMPA_vect){

  /* Moving-average baseline value, auto-adjusts to environment */
  baselineCycleCount = (15*baselineCycleCount + chargeCycleCount + 16)  >> 4; 
  cycleDifference = chargeCycleCount - baselineCycleCount;

  /* These are useful for debugging... */
  transmitByte((uint8_t) (chargeCycleCount >> 4));
  transmitByte((uint8_t) (baselineCycleCount >> 4));
  
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
  CAP_SENSOR_DDR |= (1 << CAP_SENSOR);
  CAP_SENSOR_PORT |= (1 << CAP_SENSOR);
}

void initTimer0(void){
  TCCR0A |= (1 << WGM01);	     /* CTC mode */
  TCCR0B |= (1 << CS02)|(1 << CS00); /* 8 MHz / 1024 clock */ 
  OCR0A = 200;			     /* roughly 25 ms */ 
  TIMSK0 |= (1 << OCIE0A);      /* output compare interrupt enable*/
  sei();                        /* set enable interrupt bit */
}

void initPinChangeInterrupt(void){
  PCICR |= (1 << PCIE1);   /* enable Pin-change interrupts 1 (bank C) */
  PCMSK1 |= (1 << PCINT9); /* enable specific interrupt for our pin PC1 */
  sei();		       /* set (global) interrupt enable bit */
}


int main(void){
  // -------- Inits --------- //
  initUSART();
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
    
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}
