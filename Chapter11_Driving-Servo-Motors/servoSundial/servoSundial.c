                           /* Quasi-realtime-clock with servo sundial. */

// ------- Includes -------- //
#include "servoSundial.h"
#include "_servoSerialHelpers.h"
#include "_servoClockFunctions.h"

// -------- Global Variables --------- //
volatile uint16_t ticks;
volatile uint8_t hours = 15;                 /* arbitrary default time */
volatile uint8_t minutes = 42;
volatile uint8_t seconds = 57;

ISR(TIMER0_OVF_vect) {
     /* This is going off very frequently, so we should make it speedy */
  ticks++;
}

// -------- Functions --------- //
// Servo setup and utility functions
void initTimer1_Servo(void) {
                   /* Set up Timer1 (16bit) to give a pulse every 20ms */
  TCCR1A |= (1 << WGM11);                       /* Using Fast PWM mode */
  TCCR1B |= (1 << WGM12);                       /* counter max in ICR1 */
  TCCR1B |= (1 << WGM13);
  TCCR1B |= (1 << CS11);         /* /8 prescaling -- microsecond steps */
  TCCR1A |= (1 << COM1A1);       /* set output on PB1 / OC1A for servo */
  ICR1 = 20000;                                    /* TOP value = 20ms */
}

void enableServo(void) {
  while (TCNT1 < PULSE_OVER) {;
  }                            /* delay until pulse part of cycle done */
  SERVO_DDR |= (1 << SERVO);                    /* enable servo pulses */
}

void disableServo(void) {
  while (TCNT1 < PULSE_OVER) {;
  }                            /* delay until pulse part of cycle done */
  SERVO_DDR &= ~(1 << SERVO);                  /* disable servo pulses */
}

void setServoPosition(void) {
  uint32_t elapsedMinutes;
/* using 32 bits b/c elapsedMinutes * PULSE_RANGE will overflow 16 bits */



  elapsedMinutes = (hours - START_TIME) * 60 + minutes;
  OCR1A = PULSE_MIN + elapsedMinutes * PULSE_RANGE / (HOURS_RANGE * 60);
  enableServo();
}

int main(void) {

  // -------- Inits --------- //
  clock_prescale_set(clock_div_1);                  /* CPU clock 8 MHz */
  initUSART();
  printString("\r\nWelcome to the Servo Sundial.\r\n");
  printString("Type S to set time.\r\n");

  initTimer0_Clock();
  initTimer1_Servo();
  sei();                                   /* set enable interrupt bit */
  LED_DDR |= (1 << LED0);                             /* blinky output */
  LASER_DDR |= (1 << LASER);                    /* enable laser output */

  // ------ Event loop ------ //
  while (1) {

                                                 /* Poll clock routine */
    if (ticks == OVERFLOWS_PER_SECOND) {
      ticks = 0;
      everySecond();
    }

    pollSerial();

  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}
