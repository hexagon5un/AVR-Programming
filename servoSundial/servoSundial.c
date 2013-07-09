/* Quasi-realtime-clock with servo sundial. */

// ------- Includes -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

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
void printTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

void initTimer1_Servo(void){
  /* Set up Timer1 (16bit) to give a pulse every 20ms*/
  TCCR1A |= (1 << WGM11); /* Using Fast PWM mode */
  TCCR1B |= (1 << WGM12); /* counter max in ICR1 */
  TCCR1B |= (1 << WGM13); 
  TCCR1B |= (1 << CS11);        /* /8 prescaling -- microsecond steps*/
  ICR1 = 20000;                 /* TOP value = 20ms */
  TCCR1A |= (1 << COM1A1);      /* Direct output on PB1 / OC1A */
  DDRB |= (1 << PB1);           /* set pin direction to output */
}

void initTimer0_Clock(void){
  /* Normal mode, just used for the overflow interrupt */
  TCCR0B |= (1 << CS00);        /* 8 MHz clock = ~31250 overflows per second */
  TIMSK0 |= (1 << TOIE0);      /*  timer overflow interrupt enable*/
}

ISR(TIMER0_OVF_vect){
  /* This is going off very frequently, so we should make it speedy */
  ticks++;
}

int main(void){

  char input;
  uint8_t hours = 15;		/* arbitrary default time */
  uint8_t minutes= 59;		
  uint8_t seconds = 52;		/* just about to enable motor */
 
  // -------- Inits --------- //
  initUSART();
  printString("\r\nWelcome to the Servo Sundial.\r\n");
  printString("Type S to set time.\r\n");
  
  initTimer0_Clock();
  initTimer1_Servo();
  OCR1A = PULSE_MID;	       
  sei();                       /* set enable interrupt bit */
  LED_DDR |= (1 << LED0);      /* blinky output */
  LASER_DDR |= (1 << LASER);   /* enable laser output */

  // ------ Event loop ------ //
  while(1){     

    /* Clock routine */
    if (ticks > OVERFLOWS_PER_SECOND){ /* every second */
      LED_PORT ^= (1 << LED0); 
      ticks = 0;
      seconds++;
      if (seconds > 3){
	while(TCNT1 < 3000){;}	/* delay until pulse part of cycle done */
	DDRB &= ~(1 << PB1);            /* disable servo pulses */
      }
      if (seconds > 59){	/* every minute */
	seconds = 0;
	minutes++;
	if (minutes > 59){	/* every hour */
	  minutes = 0;
	  hours++;
	  // Handle Laser On/Off times
	  if ((hours >= START_TIME) && (hours < STOP_TIME)){
	    LASER_PORT |= (1 << LASER);
	  }
	  else{
	    LASER_PORT &= ~(1 << LASER);
	  }
	  if (hours > 23){	/* every day */
	    hours = 0;
	  }
	} /* end every hour */
	// Update servo position -- every minute
	if ((hours >= START_TIME) && (hours < STOP_TIME)){
	  OCR1A = PULSE_MIN + ((hours-START_TIME)*60 + minutes) * US_PER_MINUTE;
	  while(TCNT1 < 3000){;}	/* delay until pulse part of cycle done */
	  DDRB |= (1 << PB1);             /* servo pulses on */
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
	DDRB |= (1 << PB1);
	_delay_ms(500);
	DDRB &= ~(1 << PB1);
      }
    }

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
