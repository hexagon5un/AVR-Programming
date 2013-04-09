/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //    

// -------- Functions --------- //
static inline void initTimer0(void){
  //set_bit(TCCR0A, WGM02);       /* Phase-correct PWM mode */
  set_bit(TCCR0A, WGM00);       /* Phase-correct PWM mode, pt.2 */  
  set_bit(TCCR0B, CS00);        /* Clock with /1024 prescaler */
  //  set_bit(TCCR0B, CS00);        /* Clock with /1024 prescaler, pt.2 */
}

static inline uint8_t getNumber(void){
  // Gets a PWM value from the serial port.
  // Reads in characters, turns them into a number
  char hundreds='0'; 
  char tens='0';
  char ones = '0';
  char thisChar = '0';

  do{
    hundreds = tens;            /* shift numbers over */
    tens = ones;
    ones = thisChar;
    thisChar = receiveByte();   /* get a new character */
    transmitByte(thisChar);     /* echo */
  } while(thisChar != '\r');

  transmitByte('\n');           /* newline */
  return(100*(hundreds-'0') + 10*(tens-'0') + ones-'0');
}

static inline void setBridgeState(uint8_t bridgeA, uint8_t bridgeB){
  if (bridgeA){
    set_bit(PORTD, PD6);	
    set_bit(LED_PORT, LED0);  
  }
  else {
    clear_bit(PORTD, PD6);	
    clear_bit(LED_PORT, LED0);  
  }

  if (bridgeB){
    set_bit(PORTD, PD5);	
    set_bit(LED_PORT, LED1);  
  }
  else {
    clear_bit(PORTD, PD5);	
    clear_bit(LED_PORT, LED1);  
  }
}

static inline void setPWM(uint8_t pwmValue){
  OCR0A = pwmValue;
  OCR0B = pwmValue;
}

int main(void){
  uint8_t updateSpeed;
  uint8_t i;
  // -------- Inits --------- //
  initTimer0();
  setPWM(128);

  set_bit(DDRD, PD6);  /* now hooked up to MOSFET, output1 */
  set_bit(DDRD, PD5);  /* now hooked up to MOSFET, output2 */
  set_bit(LED_DDR, LED0);  
  set_bit(LED_DDR, LED1);  
  
  initUSART();
  sayOK();

  // ------ Event loop ------ //
  while(1){     
    
    // updateSpeed = getNumber();
    // OCR0A = updateSpeed;
    // OCR0B = updateSpeed;
    //    TCCR0A &= 0b00001111;	/* clear all output com bits */
    // TCCR0A &= ~(BV(COM0A0) | BV(COM0A1) | BV(COM0B1));

    /*   setBridgeState(1,0);
    _delay_ms(1000);
    
    setBridgeState(0,0);
    _delay_ms(1000);
    
    setBridgeState(0,1);
    _delay_ms(1000);
    
    setBridgeState(1,1);
    _delay_ms(1000);

    // Brake stops
    setBridgeState(1,0);
    _delay_ms(1000);
    */

    TCCR0A |= (BV(COM0A0) | BV(COM0A1) | BV(COM0B1)); /* turn PWM on */
    //setPWM(128);
    //_delay_ms(1000);
    
    for (i=0; i<255; i++){
      setPWM(i);
      _delay_ms(60);
    }
    _delay_ms(1500);

    for (i=254; i>0; i--){
      setPWM(i);
      _delay_ms(60);
    }
    _delay_ms(1500);


  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

