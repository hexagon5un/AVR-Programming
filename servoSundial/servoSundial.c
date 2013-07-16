/* Quasi-realtime-clock with servo sundial. */

// ------- Includes -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "USART.h"

// ------- Defines -------- //
#define PULSE_MIN         1000  /* experiment with these values */
#define PULSE_MAX         2000  /* to match your own servo */
#define PULSE_RANGE       (PULSE_MAX - PULSE_MIN)

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

// -------- Functions --------- //
void printTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
void pollSerial(void);

// Servo setup and utility functions
void initTimer1_Servo(void){
  /* Set up Timer1 (16bit) to give a pulse every 20ms*/
  TCCR1A |= (1 << WGM11); /* Using Fast PWM mode */
  TCCR1B |= (1 << WGM12); /* counter max in ICR1 */
  TCCR1B |= (1 << WGM13); 
  TCCR1B |= (1 << CS11);      /* /8 prescaling -- microsecond steps*/
  TCCR1A |= (1 << COM1A1);    /* set output on PB1 / OC1A for servo */
  ICR1 = 20000;               /* TOP value = 20ms */
}

void enableServo(void){
  while(TCNT1 < 3000){;}	/* delay until pulse part of cycle done */          
  SERVO_DDR |= (1 << SERVO);	/* enable servo pulses */
}

void disableServo(void){
  while(TCNT1 < 3000){;}	/* delay until pulse part of cycle done */
   SERVO_DDR &= ~(1 << SERVO);	/* disable servo pulses */
}

void setServoPosition(void){
  uint32_t elapsedMinutes;  	
  /* using 32 bits b/c elapsedMinutes * PULSE_RANGE will overflow 16 bits */
  elapsedMinutes = (hours-START_TIME)*60 + minutes;   
  OCR1A = PULSE_MIN + elapsedMinutes * PULSE_RANGE/(HOURS_RANGE * 60);
  enableServo();
}

// Realtime-clock handling functions
void initTimer0_Clock(void){
  /* Normal mode, just used for the overflow interrupt */
  TCCR0B |= (1 << CS00);        /* 8 MHz clock = ~31250 overflows per second */
  TIMSK0 |= (1 << TOIE0);      /*  timer overflow interrupt enable*/
}

ISR(TIMER0_OVF_vect){
  /* This is going off very frequently, so we should make it speedy */
  ticks++;
}

void everyHour(void){
  hours++;
  if (hours > 23){		/* loop around at end of day */
    hours = 0;
  }
}
void everyMinute(void){
  minutes++;
  if (minutes > 59){	
    minutes = 0;
    everyHour();
  }
  // If during business hours, set servo to new minute 
  // Otherwise, don't need to move motor when laser is off
  if ((hours >= START_TIME) && (hours < STOP_TIME)){
    setServoPosition();
    enableServo();
    LASER_PORT |= (1 << LASER);	/* make sure laser is on */
  }
  else{			       /* make sure laser is off */
    LASER_PORT &= ~(1 << LASER);
  }
}
void everySecond(void){
  seconds++;
  if (seconds > 59){	
    seconds = 0;
    everyMinute(); 
  }
  LED_PORT ^= (1 << LED0);	      /* blink */
  printTime(hours, minutes, seconds); /* serial output */
  /* Turn off servo motor after three seconds into new minute */
  if (seconds == 3){
    disableServo();
  }
}

int main(void){

  // -------- Inits --------- //
  initUSART();
  printString("\r\nWelcome to the Servo Sundial.\r\n");
  printString("Type S to set time.\r\n");

  initTimer0_Clock();
  initTimer1_Servo();
  sei();                       /* set enable interrupt bit */
  LED_DDR |= (1 << LED0);      /* blinky output */
  LASER_DDR |= (1 << LASER);   /* enable laser output */

  // ------ Event loop ------ //
  while(1){     
    
    /* Poll clock routine */
    if (ticks == OVERFLOWS_PER_SECOND){ 
      ticks = 0;
      everySecond();
    }
    
    pollSerial();

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

void printTime(uint8_t hours, uint8_t minutes, uint8_t seconds){
  printByte(hours);
  transmitByte(':');
  printByte(minutes);
  transmitByte(':');
  printByte(seconds);
  transmitByte('\r');
  transmitByte('\n');
}

void pollSerial(void){
  /* Poll for serial input -- to set the time. */
  char input;
  if (bit_is_set(UCSR0A, RXC0)){
    input = UDR0;
    if (input == 'S'){		/* enter set-time mode */
      printString("Setting time...\r\n");
      printString("Hour: ");
      hours = getNumber();
      printString("\r\nMinutes: ");
      minutes = getNumber();
      printString("\r\nSeconds: ");
      seconds = getNumber();      
      printString("\r\n");
      ticks = 0;
      if ((hours >= START_TIME) && (hours < STOP_TIME)){
	setServoPosition();
      }
    }
  }  
}
