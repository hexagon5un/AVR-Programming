/* Quasi-realtime-clock with servo sundial. */

// ------- Includes -------- //
#include <avr/io.h>             

// ------- Defines -------- //
#define PULSE_MIN         1000  /* experiment with these values */
#define PULSE_MAX         2000  /* to match your own servo */
#define PULSE_RANGE       (PULSE_MAX - PULSE_MIN)
#define PULSE_OVER        3000     /* Must be larger than PULSE_MAX */ 

#define START_TIME        10	/* 10 am */
#define STOP_TIME 	  22	/* 10 pm */
#define HOURS_RANGE        (STOP_TIME - START_TIME - 1)

#define LASER             PB2
#define LASER_PORT        PORTB
#define LASER_DDR         DDRB

#define SERVO             PB1
#define SERVO_PORT        PORTB
#define SERVO_DDR         DDRB

#define OVERFLOWS_PER_SECOND   31250      /* nominal, should calibrate */

// -------- Global Variables --------- //    
volatile uint16_t ticks;
volatile uint8_t  hours   = 15;		/* arbitrary default time */
volatile uint8_t  minutes = 42;		
volatile uint8_t  seconds = 57;		

// Serial input and output functions
void pollSerial(void);
void printTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

// Servo setup and utility functions
void initTimer1_Servo(void);
void enableServo(void);
void disableServo(void);
void setServoPosition(void);

// Realtime-clock handling functions
// Use the globals ticks, hours, minutes, seconds
void initTimer0_Clock(void);
void everyHour(void);
void everyMinute(void);
void everySecond(void);
