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
#define MIN_DELAY      8                /* determines max cruise speed */
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

int main(void) {
  // -------- Inits --------- //
  initUSART();
  _delay_ms(1000);
  initTimer();
  DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

  // ------ Event loop ------ //
  while (1) {

    direction = FORWARD;
    takeSteps(TURN / 2, MIN_DELAY * 2);
    _delay_ms(1000);

    direction = BACKWARD;
    takeSteps(TURN, MIN_DELAY);
    _delay_ms(1000);

    direction = FORWARD;
    takeSteps(TURN / 16, MAX_DELAY);
    direction = BACKWARD;
    takeSteps(TURN / 8, MAX_DELAY / 2);
    direction = FORWARD;
    takeSteps(TURN / 16, MAX_DELAY);
    _delay_ms(1000);

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
