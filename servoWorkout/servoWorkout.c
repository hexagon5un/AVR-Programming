/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define NUM_SERVO_PULSES     60 /* how many repetitions to send */
#define BETWEEN_PULSE_DELAY  20 	/* ms */

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

static inline void moveTo(uint16_t servoPulseLength){
   uint8_t i;
   uint16_t j;

   for (i=0; i<NUM_SERVO_PULSES; i++){
     set_bit(PORTB, PB1);
     j=0;
     while(j < servoPulseLength){
       _delay_us(10);
       j += 10;
     }
     clear_bit(PORTB, PB1);
     _delay_ms(BETWEEN_PULSE_DELAY);
   }
}

int main(void){

  // -------- Inits --------- //
  uint8_t i;
  uint16_t servoPulseLength;
  DDRB |= BV(PB1);
  
  initUSART();
  transmitString("\r\nWelcome to the Servo Tester\r\n");
  transmitString("Centering...\r\n");
  moveTo(1500);

  // ------ Event loop ------ //
  while(1){     

    transmitString("\r\nEnter a four-digit pulse length:\r\n");
    servoPulseLength = getNumber();
    transmitString("On my way....\r\n");
    moveTo(servoPulseLength);
   
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

