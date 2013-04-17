/* Quasi-realtime-clock with servo sundial. */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define PULSE_MIN          400  /* experiment with these values */
#define PULSE_MAX         2250  /* to match your own servo */
#define PULSE_MID         1325

// -------- Global Variables --------- //    
#define OVERFLOWS_PER_SECOND   31250     /* nominal, will be off by a bit */

volatile uint16_t ticks;
volatile uint8_t seconds;
volatile uint8_t minutes;
volatile uint8_t hours;

// -------- Functions --------- //

static inline void initTimer1_Servo(void){
  /* Set up Timer1 (16bit) to give a pulse every 20ms*/
  set_bit(TCCR1A, WGM11); /* Using Fast PWM mode */
  set_bit(TCCR1B, WGM12); /* counter max in ICR1 */
  set_bit(TCCR1B, WGM13); 
 
  set_bit(TCCR1B, CS11);        /* /8 prescaling -- counting in microseconds*/
  ICR1 = 20000;                 /* TOP value = 20ms */

  set_bit(TCCR1A, COM1A1);      /* Direct output on PB1 / OC1A */
  set_bit(DDRB, PB1);           /* set pin direction to output */
}

static inline void initTimer0_Clock(void){
  /* Normal mode */
  set_bit(TCCR0B, CS00);        /* 8 MHz clock = ~31250 overflows per second */
  set_bit(TIMSK0, TOIE0);      /*  timer overflow interrupt enable*/
}

ISR(TIMER0_OVF_vect){
  /* Generic clock routine */
  ticks++;
  
  if (ticks > OVERFLOWS_PER_SECOND){
    toggle_bit(LED_PORT, LED0);
    ticks = 0;
    seconds++;
    if (seconds > 59){
      seconds = 0;
      minutes++;
      if (minutes > 59){
	minutes = 0;
	hours++;
	if (hours > 23){
	  hours = 0;
	}
      }
    } 
  }
  //clear_bit(LED_PORT, LED0);
}

int main(void){

  uint8_t i;

  // -------- Inits --------- //
  initUSART();
  sayOK();  
  
  initTimer0_Clock();
  initTimer1_Servo();
  OCR1A = PULSE_MID;
  sei();                       /* set enable interrupt bit */
  set_bit(LED_DDR, LED0);      /* test output */
  
  // ------ Event loop ------ //
  while(1){     
    
    
    // transmitByte('0'+ticks);
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

