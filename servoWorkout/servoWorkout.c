/* Quick interactive demo running servo with Timer 1 */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "UART.h"

#define PULSE_MIN          500	/* experiment with these values */
#define PULSE_MAX         2400	/* to match your own servo */
#define PULSE_MID         1325

static inline uint16_t getNumber(void);

static inline void initTimer1Servo(void){
  /* Set up Timer1 (16bit) to give a pulse every 20ms*/
  set_bit(TCCR1A, WGM11); /* Using Fast PWM mode */
  set_bit(TCCR1B, WGM12); /* counter max in ICR1 */
  set_bit(TCCR1B, WGM13); 
 
  set_bit(TCCR1B, CS11);        /* /8 prescaling -- counting in microseconds*/
  ICR1 = 20000;                 /* TOP value = 20ms */

  set_bit(TCCR1A, COM1A1);	/* Direct output on PB1 / OC1A */
  set_bit(DDRB, PB1);		/* set pin direction to output */
}

static inline void showOff(void){
  printString("Center\r\n");
  OCR1A = PULSE_MID;
  _delay_ms(1500);
  printString("Clockwise Max\r\n");
  OCR1A = PULSE_MIN;
  _delay_ms(1500);
  printString("Counterclockwise Max\r\n");
  OCR1A = PULSE_MAX;
  _delay_ms(1500);
  printString("Center\r\n");  
  OCR1A = PULSE_MID;
  _delay_ms(1500);
}

int main(void){

  // -------- Inits --------- //
  uint16_t servoPulseLength;
  OCR1A = PULSE_MID;  		/* set it to middle position initially */
  initTimer1Servo();                 
  initUART();
  printString("\r\nWelcome to the Servo Demo\r\n");
  showOff();
 
  // ------ Event loop ------ //
  while(1){     

    printString("\r\nEnter a four-digit pulse length:\r\n");
    servoPulseLength = getNumber();

    printString("On my way....\r\n");
    OCR1A = servoPulseLength;
    set_bit(DDRB, PB1);		/* re-enable output pin */

    _delay_ms(1000);
    printString("Releasing...\r\n");
    while(TCNT1 < 3000){;}	/* delay until pulse part of cycle done */
    clear_bit(DDRB, PB1);	/* disable output pin */

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

static inline uint16_t getNumber(void){
  // Gets a PWM value from the serial port.
  // Reads in characters, turns them into a number
  char thousands='0'; 
  char hundreds='0'; 
  char tens='0';
  char ones = '0';
  char thisChar = '0';

  do{
    thousands = hundreds; /* shift numbers over */
    hundreds = tens;            
    tens = ones;
    ones = thisChar;
    thisChar = receiveByte();   /* get a new character */
    transmitByte(thisChar);     /* echo */
  } while(thisChar != '\r');

  transmitByte('\n');           /* newline */
  return(1000*(thousands -'0') + 100*(hundreds-'0') + 10*(tens-'0') + ones-'0');
}
