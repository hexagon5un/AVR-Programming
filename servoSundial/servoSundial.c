/* Quasi-realtime-clock with servo sundial. */

// ------- Includes -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "UART.h"

// ------- Defines -------- //
#define PULSE_MIN          500  /* experiment with these values */
#define PULSE_MAX         2000  /* to match your own servo */
#define PULSE_MID         1500
#define PULSE_RANGE       (PULSE_MAX - PULSE_MIN)

#define START_TIME        10	/* 10 am */
#define STOP_TIME 	  22	/* 10 pm */
#define TIME_RANGE        (STOP_TIME - START_TIME - 1)

#define US_PER_MINUTE     (PULSE_RANGE / TIME_RANGE / 60)

#define LASER             PB2
#define LASER_PORT        PORTB
#define LASER_DDR         DDRB

#define OVERFLOWS_PER_SECOND   31250     /* nominal, should calibrate */

// -------- Global Variables --------- //    
volatile uint16_t ticks;

// -------- Functions --------- //
static inline uint8_t getNumber(void);
static inline void printTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

static inline void initTimer1_Servo(void){
  /* Set up Timer1 (16bit) to give a pulse every 20ms*/
  set_bit(TCCR1A, WGM11); /* Using Fast PWM mode */
  set_bit(TCCR1B, WGM12); /* counter max in ICR1 */
  set_bit(TCCR1B, WGM13); 
  set_bit(TCCR1B, CS11);        /* /8 prescaling -- microsecond steps*/
  ICR1 = 20000;                 /* TOP value = 20ms */
  set_bit(TCCR1A, COM1A1);      /* Direct output on PB1 / OC1A */
  set_bit(DDRB, PB1);           /* set pin direction to output */
}

static inline void initTimer0_Clock(void){
  /* Normal mode, just used for the overflow interrupt */
  set_bit(TCCR0B, CS00);        /* 8 MHz clock = ~31250 overflows per second */
  set_bit(TIMSK0, TOIE0);      /*  timer overflow interrupt enable*/
}

ISR(TIMER0_OVF_vect){
  /* This is going off very frequently, so we should make it speedy */
  ticks++;
}

int main(void){

  uint8_t i;
  char input;
  uint8_t hours = 15;		/* arbitrary default time */
  uint8_t minutes= 59;		
  uint8_t seconds = 52;		/* just about to enable motor */
 
  // -------- Inits --------- //
  initUART();
  printString("\r\nWelcome to the Servo Sundial.\r\n");
  printString("Type S to set time.\r\n");
  
  initTimer0_Clock();
  initTimer1_Servo();
  OCR1A = PULSE_MID;	       
  sei();                       /* set enable interrupt bit */
  set_bit(LED_DDR, LED0);      /* blinky output */
  set_bit(LASER_DDR, LASER);   /* enable laser output */

  // ------ Event loop ------ //
  while(1){     

    /* Clock routine */
    if (ticks > OVERFLOWS_PER_SECOND){ /* every second */
      toggle_bit(LED_PORT, LED0); 
      ticks = 0;
      seconds++;
      if (seconds > 3){
	while(TCNT1 < 3000){;}	/* delay until pulse part of cycle done */
	clear_bit(DDRB, PB1);            /* disable servo pulses */
      }
      if (seconds > 59){	/* every minute */
	seconds = 0;
	minutes++;
	if (minutes > 59){	/* every hour */
	  minutes = 0;
	  hours++;
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
	} /* end every hour */
	// Update servo position -- every minute
	if ((hours >= START_TIME) && (hours < STOP_TIME)){
	  OCR1A = PULSE_MIN + ((hours-START_TIME)*60 + minutes) * US_PER_MINUTE;
	  while(TCNT1 < 3000){;}	/* delay until pulse part of cycle done */
	  set_bit(DDRB, PB1);             /* servo pulses on */
	}
      } /* end every minute */
      printTime(hours, minutes, seconds);
    } /* end every second */
    
    /* Poll for serial input -- to set the time. */
    if (bit_is_set(UCSR0A, RXC0)){
      input = UDR0;
      if (input == 'S'){		/* enter set-time mode */
	printString("Setting time...\r\n");
	printString("Hour: ");
	hours = getNumber();
	printString("Minutes: ");
	minutes = getNumber();
	printString("Seconds: ");
	seconds = getNumber();      
	// Update position now for instant gratification
	OCR1A = PULSE_MIN + ((hours-START_TIME)*60 + minutes) * US_PER_MINUTE;
	set_bit(DDRB, PB1);
	_delay_ms(500);
	clear_bit(DDRB, PB1);
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

  printString('\r\n');           /* newline */
  return(100*(hundreds-'0') + 10*(tens-'0') + ones-'0');
}

static inline void printTime(uint8_t hours, uint8_t minutes, uint8_t seconds){
  printByte(hours);
  transmitByte(':');
  printByte(minutes);
  transmitByte(':');
  printByte(seconds);
  transmitByte('\r');
  transmitByte('\n');
}
