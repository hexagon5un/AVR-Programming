/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#define FORWARD    1
#define BACKWARD   -1
#define MAX_DELAY  255
#define MIN_DELAY  15

#define ACCELERATION  16
#define RAMP_STEPS    (MAX_DELAY - MIN_DELAY) / ACCELERATION

// -------- Global Variables --------- //  

volatile uint8_t stepPhase = 0;
volatile int8_t  direction = FORWARD;
volatile int16_t numSteps = 0;

const uint8_t stepOrder[] = {
  (1<<PB0) | (1<<PB2),
  (1<<PB0) | (1<<PB3),
  (1<<PB1) | (1<<PB3),
  (1<<PB1) | (1<<PB2)
};
#define LAST_PHASE_IN_CYCLE  3

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
    if (stepPhase < LAST_PHASE_IN_CYCLE){
      stepPhase++;
    }
    else{
      stepPhase = 0;
    }
  }
  else{ 			/* backward */
    if (stepPhase > 0){
      stepPhase--;
    }
    else{
      stepPhase = LAST_PHASE_IN_CYCLE;
    }
  }
  PORTB = stepOrder[stepPhase];
  numSteps++;
}

void takeSteps(uint16_t howManySteps, uint8_t delay){
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
  uint8_t i;
 
  initUSART();
  initTimer();
  DDRB = (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3);
  
  // ------ Event loop ------ //
  while(1){     

    trapezoidMove(384);		/* full speed for a while */
    _delay_ms(1000);    
    trapezoidMove(93);		/* short period full speed */
    _delay_ms(1000);    
    trapezoidMove(23);		/* never reaches full speed */
    _delay_ms(1000);    
    trapezoidMove(-93);        /* short period full speed, backward */
    _delay_ms(1000);    
    trapezoidMove(-23);	      /* never reaches full speed, backward */
    _delay_ms(1000);    
    trapezoidMove(-384);	/* full speed for a while, backward */
    _delay_ms(1000);    
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

