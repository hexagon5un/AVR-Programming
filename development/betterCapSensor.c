/* Includes */
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>         
#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

/* Global Variables */
volatile uint16_t chargeCycleCount;

ISR(TIMER0_COMPA_vect){
  /* run when timer compare match */
  transmitByte((uint8_t) (chargeCycleCount >> 4));

  if (chargeCycleCount == 0){	/* trigger charge/discharge cycle */
    set_bit(CAP_SENSOR_DDR, CAP_SENSOR); 
    set_bit(CAP_SENSOR_PORT, CAP_SENSOR);
  }

  chargeCycleCount = 0;
  toggle_bit(LED_PORT, LED1);
}

static inline void initTimer0(void){
  set_bit(TCCR0A, WGM01);       /* CTC mode */
  set_bit(TCCR0B, CS02);        /* 8 MHz / 8 clock */
  set_bit(TCCR0B, CS00);        /* 8 MHz / 8 clock */
  set_bit(TIMSK0, OCIE0A);      /* output compare interrupt enable*/
  OCR0A = 200;                   /* 8 Mhz / 8 /  36 =  */ 
  sei();                        /* set enable interrupt bit */
}

ISR(PCINT1_vect){
  set_bit(LED_PORT, LED0);
  chargeCycleCount++;		/* count new value */
  
  clear_bit(CAP_SENSOR_PORT, CAP_SENSOR); /* set low to discharge capacitor */
  set_bit(CAP_SENSOR_DDR, CAP_SENSOR); /* output mode */
  _delay_us(1);			  /* a short while */
    
  clear_bit(CAP_SENSOR_DDR, CAP_SENSOR); /* set as input */
  set_bit(CAP_SENSOR_PORT, CAP_SENSOR);  /* set pullup to charge capacitor */
  set_bit(PCIFR, PCIF1); 
  clear_bit(LED_PORT, LED0);
  cli();
}

static inline void initPinChangeInterrupt(void){
  /* B, C, D = IE0, IE1, IE2 */
  set_bit(PCICR, PCIE1); /* enable Pin-change interrupts 1 (bank C) */
  set_bit(PCMSK1, PCINT9); /* enable specific interrupt for our pin PC1 */
  sei();		       /* set (global) interrupt enable bit */
}


int main(void){
  

  // -------- Inits --------- //
  initUSART();
  printString("Better Capacitive Sensor\r\n");
  LED_DDR = 0xff;
  initTimer0();
  initPinChangeInterrupt();

  // ------ Event loop ------ //
  while(1){			
    
    ;

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}
