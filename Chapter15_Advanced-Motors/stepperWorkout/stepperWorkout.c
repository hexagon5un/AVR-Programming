                          /* Stepper Motor Demo with Accelerated Moves */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

      /* Set these to +/- 1 for half-stepping, +/- 2 for full-stepping */
#define FORWARD    1
#define BACKWARD  -1
#define TURN       400          /* steps per rotation,
                                   depends on stepping & motor */

      /* These parameters will depend on your motor, what it's driving */
#define MAX_DELAY    255               /* determines min startup speed */
#define MIN_DELAY     10                /* determines max cruise speed */
#define ACCELERATION  16          /* lower = smoother but slower accel */

#define RAMP_STEPS    (MAX_DELAY - MIN_DELAY) / ACCELERATION

// -------- Global Variables --------- //
const uint8_t motorPhases[] = {
  (1 << PB0) | (1 << PB2),                                     /* full */
  (1 << PB0),                                                  /* half */
  (1 << PB0) | (1 << PB3),                                     /* full */
  (1 << PB3),                                                  /* half */
  (1 << PB1) | (1 << PB3),                                     /* etc. */
  (1 << PB1),
  (1 << PB1) | (1 << PB2),
  (1 << PB2)
};

volatile uint8_t stepPhase = 0;
volatile int8_t direction = FORWARD;
volatile uint16_t stepCounter = 0;

// -------- Functions --------- //
void initTimer(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0B |= (1 << CS00) | (1 << CS02);
  OCR0A = MAX_DELAY;                   /* set default speed as slowest */
  sei();                                   /* enable global interrupts */
               /* Notice we haven't set the timer0 interrupt flag yet. */
}

ISR(TIMER0_COMPA_vect) {
  stepPhase += direction;              /* take step in right direction */
  stepPhase &= 0b00000111;                  /* keep phase in range 0-7 */
  PORTB = motorPhases[stepPhase];          /* write phase out to motor */
  stepCounter++;                                   /* count step taken */
}

void takeSteps(uint16_t howManySteps, uint8_t delay) {
  UDR0 = delay;          /* send speed/delay over serial, non-blocking */
  OCR0A = delay;                  /* delay in counter compare register */
  stepCounter = 0;            /* initialize to zero steps taken so far */
  TIMSK0 |= (1 << OCIE0A);             /* turn on interrupts, stepping */
  while (!(stepCounter == howManySteps)) {;
  }                                                            /* wait */
  TIMSK0 &= ~(1 << OCIE0A);                           /* turn back off */
}

void trapezoidMove(int16_t howManySteps) {
  uint8_t delay = MAX_DELAY;
  uint16_t stepsTaken = 0;

                               /* set direction, make howManySteps > 0 */
  if (howManySteps > 0) {
    direction = FORWARD;
  }
  else {
    direction = BACKWARD;
    howManySteps = -howManySteps;
  }

  if (howManySteps > (RAMP_STEPS * 2)) {
                             /* Have enough steps for a full trapezoid */
                                                         /* Accelerate */
    while (stepsTaken < RAMP_STEPS) {
      takeSteps(1, delay);
      delay -= ACCELERATION;
      stepsTaken++;
    }
                                                             /* Cruise */
    delay = MIN_DELAY;
    takeSteps((howManySteps - 2 * RAMP_STEPS), delay);
    stepsTaken += (howManySteps - 2 * RAMP_STEPS);
                                                         /* Decelerate */
    while (stepsTaken < howManySteps) {
      takeSteps(1, delay);
      delay += ACCELERATION;
      stepsTaken++;
    }
  }
  else {
                                               /* Partial ramp up/down */
    while (stepsTaken <= howManySteps / 2) {
      takeSteps(1, delay);
      delay -= ACCELERATION;
      stepsTaken++;
    }
    delay += ACCELERATION;
    while (stepsTaken < howManySteps) {
      takeSteps(1, delay);
      delay += ACCELERATION;
      stepsTaken++;
    }
  }
}

int main(void) {
  // -------- Inits --------- //
  initUSART();
  _delay_ms(1000);
  initTimer();
  DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

  // ------ Event loop ------ //
  while (1) {

                         /* Smooth movements, trapezoidal acceleration */
    trapezoidMove(2 * TURN);                         /* two full turns */
    trapezoidMove(-TURN / 2);                             /* half turn */
    trapezoidMove(TURN / 4);                           /* quarter turn */
    trapezoidMove(-TURN / 8);                                /* eighth */
    _delay_ms(TURN);
    trapezoidMove(-TURN / 4);                         /* the other way */
    trapezoidMove(TURN / 8);
    trapezoidMove(TURN / 2);                /* half turn back to start */
    _delay_ms(1000);

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
