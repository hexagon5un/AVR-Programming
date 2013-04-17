/* Quasi-realtime-clock with servo sundial. */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define PULSE_MIN          400  /* experiment with these values */
#define PULSE_MAX         2250  /* to match your own servo */
#define PULSE_MID         1325
#define PULSE_RANGE       (PULSE_MAX - PULSE_MIN)

#define START_TIME        10	/* 10 am */
#define STOP_TIME 	  20	/* 8 pm */
#define TIME_RANGE        (STOP_TIME - START_TIME)

#define MS_PER_MINUTE     (PULSE_RANGE / TIME_RANGE / 60)

#define LASER             PB2
#define LASER_PORT        PORTB
#define LASER_DDR         DDRB

// -------- Global Variables --------- //    
#define OVERFLOWS_PER_SECOND   31250     /* nominal, will be off by a bit */

volatile uint16_t ticks;

// -------- Functions --------- //
static inline uint8_t getNumber(void);

static inline void initTimer1_Servo(void){
  /* Set up Timer1 (16bit) to give a pulse every 20ms*/
  set_bit(TCCR1A, WGM11); /* Using Fast PWM mode */
  set_bit(TCCR1B, WGM12); /* counter max in ICR1 */
  set_bit(TCCR1B, WGM13); 
 
  set_bit(TCCR1B, CS11);        /* /8 prescaling -- counting in microseconds*/
  ICR1 = 20000;                 /* TOP value = 20ms */

  set_bit(TCCR1A, COM1A1);      /* Direct output on PB1 / OC1A */
  set_bit(DDRB, PB1);           /* set pin direction to output */
}

static inline void initTimer0_Clock(void){
  /* Normal mode */
  set_bit(TCCR0B, CS00);        /* 8 MHz clock = ~31250 overflows per second */
  set_bit(TIMSK0, TOIE0);      /*  timer overflow interrupt enable*/
}

ISR(TIMER0_OVF_vect){
  /* This is going off very frequently, so we should make it speedy */
  ticks++;
}

static inline void blinkLaser(uint8_t times){
  while(times){
    set_bit(LASER_PORT, LASER);
    _delay_ms(200);
    clear_bit(LASER_PORT, LASER);
    _delay_ms(100);
    times--;
  }
}

int main(void){

  char input;
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t i;

  // -------- Inits --------- //
  initUSART();
  sayOK();  
  
  initTimer0_Clock();
  initTimer1_Servo();
  OCR1A = PULSE_MID;	       
  sei();                       /* set enable interrupt bit */
  set_bit(LED_DDR, LED0);      /* blinky output */
  set_bit(LASER_DDR, LASER);   /* enable laser output */
  blinkLaser(5);               /* blink to test */

  // ------ Event loop ------ //
  while(1){     
    /* Simple clock routine */
    if (ticks > OVERFLOWS_PER_SECOND){ /* every second */
      toggle_bit(LED_PORT, LED0); 
      ticks = 0;
      seconds++;
      if (seconds > 3){
	clear_bit(TCCR1A, COM1A1);  /* motor off, save power if it seeks */
      }
      if (seconds > 59){	/* every minute */
	seconds = 0;
	minutes++;
	// Update servo position
	if ((hours >= START_TIME) && (hours < STOP_TIME)){
	  set_bit(TCCR1A, COM1A1);  /* motor on */
	  OCR1A = PULSE_MIN + ((hours-START_TIME)*60 + minutes) * MS_PER_MINUTE;
	}
	if (minutes > 59){	/* every hour */
	  minutes = 0;
	  hours++;
	  blinkLaser(5);
	  // Handle Laser On/Off times
	  if ((hours >= START_TIME) && (hours < STOP_TIME)){
	    set_bit(LASER_PORT, LASER);
	  }
	  else{
	    clear_bit(LASER_PORT, LASER);
	  }
	  if (hours > 23){	/* every day */
	    hours = 0;
	  }
	}
      } 
      
      // Transmit time over serial for debugging, timing
      // Still in "every second" loop
      printByte(hours);
      transmitByte(':');
      printByte(minutes);
      transmitByte(':');
      printByte(seconds);
      transmitByte('\r');
      transmitByte('\n');
    }
    
    // Poll for serial input -- allow for setting time.
    if (bit_is_set(UCSR0A, RXC0)){
      input = UDR0;
      if (input == 'S'){		/* enter set-time mode */
	transmitString("Setting time...\r\n");
	transmitString("Hour: ");
	hours = getNumber();
	transmitString("Minutes: ");
	minutes = getNumber();
	transmitString("Seconds: ");
	seconds = getNumber();      
	// Update position now for instant gratification
	OCR1A = PULSE_MIN + ((hours-START_TIME)*60 + minutes) * MS_PER_MINUTE;
      }
    }

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}


static inline uint8_t getNumber(void){
  // Gets a numerical 0-255 from the serial port.
  // Converts from string to number.
  char hundreds='0'; 
  char tens='0';
  char ones = '0';
  char thisChar = '0';
  do{    
    hundreds = tens;            
    tens = ones;
    ones = thisChar;
    thisChar = receiveByte();   /* get a new character */
    transmitByte(thisChar);     /* echo */
  } while(thisChar != '\r');

  transmitString('\r\n');           /* newline */
  return(100*(hundreds-'0') + 10*(tens-'0') + ones-'0');
}
