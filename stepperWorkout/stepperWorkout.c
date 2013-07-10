/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#define FORWARD  1
#define BACKWARD -1
#define MAX_DELAY  220
#define MIN_DELAY  20
#define STEPS_PER_SPEED  4

#define DEBUG   1    /* comment this out if you don't want printout */

const uint8_t stepOrder[] = {
  (1<<PB0) | (1<<PB2),
  (1<<PB0) | (1<<PB3),
  (1<<PB1) | (1<<PB3),
  (1<<PB1) | (1<<PB2)
};
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
    if (step < 3){
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
      step = 3;
    }
  }
  PORTB = stepOrder[step];
  numSteps++;
}

void takeSteps(uint16_t howMany, uint8_t delay){
#ifdef DEBUG
  UDR0 = delay;			/* transmit the byte out, non-blocking */
#endif
  OCR0A = delay;
  numSteps = 0;
  TIMSK0 |= (1 << OCIE0A);
  while(numSteps < howMany){;}
  TIMSK0 &= ~(1 << OCIE0A);
}

void rampToSpeed(uint8_t targetDelay){
  uint8_t newSpeed;
  if (OCR0A > targetDelay){	/* currently slow */    
    while(( (OCR0A*7>>3) > targetDelay) && (OCR0A >= 2)){
      takeSteps(STEPS_PER_SPEED, (OCR0A * 7) >> 3);
    }
  }
  else{				/* currently fast */
    while(( (OCR0A*9>>3) < targetDelay) && (OCR0A < 226)){  
      takeSteps(STEPS_PER_SPEED, (OCR0A * 9) >> 3);
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

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

