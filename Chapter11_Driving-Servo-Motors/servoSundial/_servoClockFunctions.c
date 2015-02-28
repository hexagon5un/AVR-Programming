#include "_servoClockFunctions.h"

// Realtime-clock handling functions
void initTimer0_Clock(void) {
                  /* Normal mode, just used for the overflow interrupt */
  TCCR0B |= (1 << CS00);  /* 8 MHz clock = ~31250 overflows per second */
  TIMSK0 |= (1 << TOIE0);           /* timer overflow interrupt enable */
}

void everySecond(void) {
  seconds++;
  if (seconds > 59) {
    seconds = 0;
    everyMinute();
  }
  LED_PORT ^= (1 << LED0);                                    /* blink */
  printTime(hours, minutes, seconds);                 /* serial output */
           /* Turn off servo motor after three seconds into new minute */
  if (seconds == 3) {
    disableServo();
  }
}
void everyMinute(void) {
  minutes++;
  if (minutes > 59) {
    minutes = 0;
    everyHour();
  }
  // If during business hours, set servo to new minute
  // Otherwise, don't need to move motor when laser is off
  if ((hours >= START_TIME) && (hours < STOP_TIME)) {
    setServoPosition();
    enableServo();
    LASER_PORT |= (1 << LASER);               /* make sure laser is on */
  }
  else {                                     /* make sure laser is off */
    LASER_PORT &= ~(1 << LASER);
  }
}
void everyHour(void) {
  hours++;
  if (hours > 23) {                       /* loop around at end of day */
    hours = 0;
  }
}
