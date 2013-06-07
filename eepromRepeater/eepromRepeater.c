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
volatile uint8_t  patternStep;
volatile uint8_t* selectedPattern;
volatile uint8_t playback;

//const notemap[11] = {}

uint8_t  EEMEM pat1[] = {C1,C1,C1,C1,0,0,0,0,
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
uint8_t EEMEM  pat2[] = {D1,D1,D1,D1,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D1,D1,D1,D1,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D1,D1,D1,D1,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D1,D1,D1,D1,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D2,0,0,0,0,0,0,0,
			 D2,0,0,0,0,0,0,0
};
uint8_t EEMEM pat3[] = {E1,E1,E1,E1,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E1,E1,E1,E1,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E1,E1,E1,E1,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E1,E1,E1,E1,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E2,0,0,0,0,0,0,0,
			E2,0,0,0,0,0,0,0
};
uint8_t  EEMEM pat4[]= {G1,G1,G1,G1,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G1,G1,G1,G1,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G1,G1,G1,G1,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G1,G1,G1,G1,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G2,0,0,0,0,0,0,0,
			G2,0,0,0,0,0,0,0
};

// -------- Functions --------- //
// Audio output on timer0
void initTimer0(void){
  TCCR0A |= (1<<WGM01) | (1<<COM0A0); /* CTC, output on pin */
  TCCR0B |= (1<<CS00) | (1<<CS01);    /* set prescaler */
}

//  timer2
void initTimer2(void){
  TCCR2A |= (1<<WGM21) ;	   /* CTC */
  TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20); /* set prescaler /1024 */
  OCR2A = 250;				   /* sets note speed */
  /* will enable compare-match interrupt later */
}

ISR(TIMER2_COMPA_vect){
  uint8_t currentNote = 0;
  if (patternStep < PATTERN_LEN){
    currentNote = eeprom_read_byte(selectedPattern + patternStep);
    if (currentNote && playback){
      OCR0A = currentNote;
      set_bit(SPEAKER_DDR, SPEAKER);
    }
    else{
      clear_bit(SPEAKER_DDR, SPEAKER);
    }
    patternStep++;
  }
  else{		     /* done. turn off speaker, timer interrupt */
    clear_bit(SPEAKER_DDR, SPEAKER);
    clear_bit(TIMSK2, OCIE2A);
  }
}

void playPattern(uint8_t* whichPattern){
  selectedPattern = whichPattern;
  patternStep = 0;
  playback = 1;
  set_bit(TIMSK2, OCIE2A);	      /* enable T2 interrupt */
  while(patternStep < PATTERN_LEN){;} 	/* wait until done */
}

void recordPattern(uint8_t* buffer){
  uint8_t thisStep;
  patternStep = 0;
  playback = 0;
  set_bit(TIMSK2, OCIE2A);	      /* enable T2 interrupt */
  while(patternStep < PATTERN_LEN){	/* loop until done */
    if (bit_is_set(UCSR0A, RXC0)){	/* just got bit */
      buffer[patternStep] = UDR0;
      thisStep = patternStep;
      while(thisStep == patternStep){;} /* wait until next step */
    }
    else{
      buffer[patternStep] = 0;
    }
  }
  
}

int main(void){

  // -------- Inits --------- //
  uint8_t recordBuffer[PATTERN_LEN];

  initTimer0();
  initTimer2();
  sei();
  initUART();
  set_bit(LED_DDR, LED0);
  
  printString("\r\n------  Playback / Recorder  ------\r\n");
  printString(" [r] + 1-4 to record a track\r\n");
  printString(" [p] + 1-4 to play a track once\r\n");
  printString(" [l] + 1-4 to play a track looped\r\n");
  printString(" [a] to play all tracks\r\n");

  playPattern(pat4);
  
  set_bit(LED_PORT, LED0);
  recordPattern(recordBuffer);
  eeprom_update_block(recordBuffer, pat1, PATTERN_LEN);
  clear_bit(LED_PORT, LED0);
  
  playPattern(pat1);
  playPattern(pat2);
  playPattern(pat3);
  playPattern(pat4);
  
  while(1){
    
    ;

  }   
  // ------ Event loop ------ //
  return(0);                  /* This line is never reached  */
}

