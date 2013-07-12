/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include "USART.h"

#define FORWARD    1
#define BACKWARD   -1
#define MAX_DELAY  255		/* this sets the min speed */
#define MIN_DELAY  20		/* this sets the max speed */

#define ACCELERATION  16     /* set this lower if motor skips steps */
#define RAMP_STEPS    (MAX_DELAY - MIN_DELAY) / ACCELERATION

// -------- Global Variables --------- //  
const uint8_t motorPhases[] = {
  (1<<PB0) | (1<<PB2),		/* full */
  (1<<PB0), 			/* half */
  (1<<PB0) | (1<<PB3),		/* full */
  (1<<PB3),			/* half */
  (1<<PB1) | (1<<PB3),		/* etc. */
  (1<<PB1),
  (1<<PB1) | (1<<PB2),
  (1<<PB2)
};
#define LAST_PHASE    7		/* the last step in the phase table */
 
volatile uint8_t stepPhase = 0;
volatile int8_t  direction = FORWARD;
volatile int16_t numSteps = 0;

// -------- Functions --------- //
void initTimer(void){
  TCCR0A |= (1 << WGM01);               /* CTC mode */
  TCCR0B |= (1 << CS00) | (1 << CS02);  
  OCR0A = MAX_DELAY;                   
  TIMSK0 |= (1 << OCIE0A);      /* output compare interrupt enable*/
  sei();                        /* set (global) enable interrupt bit */
}

ISR(TIMER0_COMPA_vect){
  if (direction == FORWARD){		
      stepPhase++;
      if (stepPhase > LAST_PHASE){
	stepPhase = 0;
      }
  }
  else{ 			/* backward */
    stepPhase--;
    if (stepPhase > LAST_PHASE){ /* wrapped around to 255 */
      stepPhase = LAST_PHASE;
    }
  }
  PORTB = motorPhases[stepPhase];
  numSteps++;
}

void takeSteps(int16_t howManySteps, uint8_t delay){
  UDR0 = delay;		     /* transmit the byte out, non-blocking */
  OCR0A = delay;
  numSteps = 0;
  TIMSK0 |= (1 << OCIE0A);
  while(numSteps < howManySteps){;}
  TIMSK0 &= ~(1 << OCIE0A);
}

void trapezoidMove(int16_t howManySteps){
  uint8_t delay = MAX_DELAY;
  uint16_t stepsTaken = 0;

  if (howManySteps > 0){
    direction = FORWARD;
  }
  else{
    direction = BACKWARD;
    howManySteps = -howManySteps;
  }

  if (howManySteps > (RAMP_STEPS*2)){
    /* Have enough steps for a full trapezoid */
    while(stepsTaken < RAMP_STEPS){
      takeSteps(1, delay);
      delay -= ACCELERATION;
      stepsTaken++;
    }
    delay = MIN_DELAY;
    while(stepsTaken < (howManySteps - RAMP_STEPS)){
      takeSteps(1, delay);
      stepsTaken++;
    }
    while(stepsTaken < howManySteps){
      takeSteps(1, delay);
      delay += ACCELERATION;
      stepsTaken++;
    }
  }
  else{
    /* Partial ramp up/down */
    while(stepsTaken <= howManySteps/2){
      takeSteps(1, delay);
      delay -= ACCELERATION;
      stepsTaken++;
    }
    delay += ACCELERATION;
    while(stepsTaken < howManySteps){
      takeSteps(1, delay);
      delay += ACCELERATION;
      stepsTaken++;
    }    
  }
}

int main(void){
  // -------- Inits --------- //
  initUSART();
  _delay_ms(5000);
  initTimer();
  DDRB = (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3);

  // ------ Event loop ------ //
  while(1){     

    /* Smooth movements, trapezoidal acceleration */
    trapezoidMove(384);		/* one full turn */
    trapezoidMove(-384/2);	/* half turn */  
    trapezoidMove(100);		/* quarter turn */
    _delay_ms(400);  
    trapezoidMove(25);		/* eighth */
    _delay_ms(400);  
    trapezoidMove(-100);        /* the other way */
    _delay_ms(400);  
    trapezoidMove(-25);	      
    _delay_ms(1000);    
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

