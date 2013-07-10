/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#define FORWARD  1
#define BACKWARD -1
#define MAX_DELAY  255
#define MIN_DELAY  20

#define STEPS_PER_SPEED  4
#define ACCELERATION     10


//  #define HALF_STEPPING  

#ifndef HALF_STEPPING
const uint8_t stepOrder[] = {
  (1<<PB0) | (1<<PB2),
  (1<<PB0) | (1<<PB3),
  (1<<PB1) | (1<<PB3),
  (1<<PB1) | (1<<PB2)
};
#define LAST_STEP_IN_CYCLE  3

#else

const uint8_t stepOrder[] = {
  (1<<PB0) | (1<<PB2),
  (1<<PB0) ,
  (1<<PB0) | (1<<PB3),
  (1<<PB3),
  (1<<PB1) | (1<<PB3),
  (1<<PB1) ,
  (1<<PB1) | (1<<PB2),
  (1<<PB2)
};
#define LAST_STEP_IN_CYCLE  7
#endif

volatile uint8_t step = 0;
volatile int8_t direction = FORWARD;
volatile int16_t numSteps = 0;

void initTimer(void){
  TCCR0A |= (1 << WGM01);               /* CTC mode */
  TCCR0B |= (1 << CS00) | (1 << CS02);  
  OCR0A = MAX_DELAY;                   
  TIMSK0 |= (1 << OCIE0A);      /* output compare interrupt enable*/
  sei();                        /* set (global) enable interrupt bit */
}

ISR(TIMER0_COMPA_vect){
  if (direction == FORWARD){		
    if (step < LAST_STEP_IN_CYCLE){
      step++;
    }
    else{
      step = 0;
    }
  }
  else{ 			/* backward */
    if (step > 0){
      step--;
    }
    else{
      step = LAST_STEP_IN_CYCLE;
    }
  }
  PORTB = stepOrder[step];
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

void trapezoidMove(uint16_t howManySteps){
  numSteps = 0;
  uint16_t rampSteps = (256 - MIN_DELAY) / ACCELERATION * 2;
  uint16_t s;
  
  if (howManySteps > rampSteps){
    /* Enough steps to ramp up to full speed and back */
    for (s = 0; s < rampSteps/2; s++){
      takeSteps(1, 256 - (s * ACCELERATION));
    }
    for (s = 0; s < howManySteps - rampSteps; s++){
      takeSteps(1, MIN_DELAY);
    }
    for (s = 0; s < rampSteps/2; s++){
      takeSteps(1, 256 - ((rampSteps/2 - s) * ACCELERATION));
    }
  }
  else{
    /* Partial ramp up/down */
    for (s = 0; s < howManySteps/2; s++){
      takeSteps(1, 256 - (s * ACCELERATION));
    }
    for (s = howManySteps/2; s < howManySteps; s++){
      takeSteps(1, 256 - ((howManySteps * s) * ACCELERATION));
    }
    
  }
}

void rampToSpeed(uint8_t targetDelay){
  uint8_t newSpeed;
  if (OCR0A > targetDelay){	/* currently slow */    
    while(( (OCR0A*7>>3) > targetDelay) && (OCR0A >= 2)){
      takeSteps(STEPS_PER_SPEED, OCR0A * 7 >> 3);
    }
  }
  else{				/* currently fast */
    while(( (OCR0A*9>>3) < targetDelay) && (OCR0A < 226)){  
      takeSteps(STEPS_PER_SPEED, OCR0A * 9 >> 3);
    }
  }
  takeSteps(STEPS_PER_SPEED, targetDelay);
}


// -------- Global Variables --------- //    

// -------- Functions --------- //

int main(void){
  // -------- Inits --------- //
  uint8_t i;
 
  initUSART();
  initTimer();
  DDRB = (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3);
  
  
  // ------ Event loop ------ //
  while(1){     

    direction = FORWARD;
    trapezoidMove(1152);
    _delay_ms(960);
    trapezoidMove(93);
    _delay_ms(960);
    direction = BACKWARD;
    trapezoidMove(93);
    _delay_ms(960);
    trapezoidMove(1152);
    _delay_ms(960);

#ifdef FOOBERT  
    rampToSpeed(MIN_DELAY);
    takeSteps(400, MIN_DELAY);		/* two turns at max speed */
    
    rampToSpeed(MAX_DELAY);
    _delay_ms(1000);
    takeSteps(50, MAX_DELAY);		/* 1/2 turn at min speed */		
    _delay_ms(1000);
    direction = BACKWARD;
    takeSteps(50, MAX_DELAY);		/* 1/2 turn at min speed */
    _delay_ms(1000);

    
    rampToSpeed(MIN_DELAY);
    takeSteps(400, MIN_DELAY);		/* two turns at max speed */
    rampToSpeed(MAX_DELAY);
#endif 

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

