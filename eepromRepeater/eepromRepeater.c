/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "pinDefines.h"
#include "macros.h"
#include "UART.h"
#include "scale8.h"             /* 8-bit scale */


// Define EEMEM variables
#define PATTERN_LEN   128	/* 16 * 8 */
uint8_t pattern1[PATTERN_LEN] EEMEM = {C1,C1,C1,C1,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C1,C1,C1,C1,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C1,C1,C1,C1,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C1,C1,C1,C1,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C2,0,0,0,0,0,0,0,
				       C2,0,0,0,0,0,0,0
				       };
uint8_t pattern2[PATTERN_LEN] EEMEM;
uint8_t pattern3[PATTERN_LEN] EEMEM;
uint8_t pattern4[PATTERN_LEN] EEMEM;
volatile uint8_t patternStep;
volatile uint8_t* selectedPattern;

// -------- Functions --------- //
// Audio output on timer0
static inline void initTimer0(void){
  TCCR0A |= (1<<WGM01) | (1<<COM0A0); /* CTC, output on pin */
  TCCR0B |= (1<<CS00) | (1<<CS01);    /* set prescaler */
}

//  timer2
static inline void initTimer2(void){
  TCCR2A |= (1<<WGM21) ;	   /* CTC */
  TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20); /* set prescaler /1024 */
  OCR2A = 255;				   /* sets note speed */
  TIMSK2 |= (1<<OCIE2A);	/* set compare interrupt */
  sei();
}

ISR(TIMER2_COMPA_vect){
  uint8_t currentNote = 0;
  if (patternStep < PATTERN_LEN){
    currentNote = eeprom_read_byte(&selectedPattern[patternStep]);
    if (currentNote){
      OCR0A = currentNote;
      set_bit(SPEAKER_DDR, SPEAKER);
    }
    else{
      clear_bit(SPEAKER_DDR, SPEAKER);
    }
    patternStep++;
  }
  else{
    patternStep = 0;
  }
  
}

int main(void){

  // -------- Inits --------- //
 
  initTimer0();
  initTimer2();
  initUART();
  printWord(&pattern1);  
  printString("\r\n");
  printWord(&pattern2);  
  printString("\r\n");
  printWord(&pattern3);  
  printString("\r\n");
  printWord(&pattern4);  
  printString("\r\n");

  while(1){
    selectedPattern = pattern1;

    _delay_ms(1000);
    _delay_ms(1000);
    _delay_ms(1000);
    //selectedPattern = pattern2;
    //selectedPattern = pattern3;
    //selectedPattern = pattern4;
   
  }   
  // ------ Event loop ------ //
  return(0);                  /* This line is never reached  */
}

