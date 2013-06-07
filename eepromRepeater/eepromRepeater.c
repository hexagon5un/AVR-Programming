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


// Some globals
volatile uint8_t  patternStep;	/* where we are in the pattern */
volatile uint8_t* selectedPattern; /* which pattern */
uint8_t  notemap[96];		   /* mapping keys to notes */

// Define EEMEM variables
#define PATTERN_LEN   128	/* 16 * 8 */
uint8_t  EEMEM pat1[PATTERN_LEN];
uint8_t  EEMEM pat2[PATTERN_LEN];
uint8_t  EEMEM pat3[PATTERN_LEN];
uint8_t  EEMEM pat4[PATTERN_LEN];

// -------- Functions --------- //
// Audio output on timer0
void initTimer0(void){
  TCCR0A |= (1<<WGM01) | (1<<COM0A0); /* CTC, output on pin */
  TCCR0B |= (1<<CS00) | (1<<CS01);    /* set prescaler */
}

//  Timer2 -- clock advances to next sample
void initTimer2(void){
  TCCR2A |= (1<<WGM21) ;	   /* CTC */
  TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20); /* set prescaler /1024 */
  OCR2A = 250;				   /* sets note speed */
  /* will enable compare-match interrupt later */
}

ISR(TIMER2_COMPA_vect){
  if (patternStep < PATTERN_LEN){
    patternStep++;    
  }
  else{	   /* done. turn off speaker, timer interrupt */
    clear_bit(SPEAKER_DDR, SPEAKER);
    clear_bit(TIMSK2, OCIE2A);
  }
}


void playPattern(uint8_t* whichPattern){
  uint8_t currentNote;
  uint8_t thisStep;
  patternStep = 0;
  set_bit(TIMSK2, OCIE2A);	      /* enable T2 interrupt */
  while(patternStep < PATTERN_LEN){
    if ((patternStep & 0b00000111) == 1){ 
      toggle_bit(LED_PORT, LED0);
    }
    currentNote = eeprom_read_byte(whichPattern + patternStep);
    if (currentNote){		/* if there is a note, play it */
      OCR0A = currentNote;
      set_bit(SPEAKER_DDR, SPEAKER);
    }
    else{			/* otherwise, turn output off */
      clear_bit(SPEAKER_DDR, SPEAKER);
    }
    thisStep = patternStep;
    while(thisStep == patternStep){;} /* wait until next step */
  } 
  clear_bit(LED_PORT, LED0);
}

void playBuffer(uint8_t buffer[]){
  uint8_t currentNote;
  uint8_t thisStep;
  patternStep = 0;
  set_bit(TIMSK2, OCIE2A);	      /* enable T2 interrupt */
  while(patternStep < PATTERN_LEN){
    if ((patternStep & 0b00000111) == 1){ 
      toggle_bit(LED_PORT, LED0);
    }
    currentNote = buffer[patternStep];
    if (currentNote){		/* if there is a note, play it */
      OCR0A = currentNote;
      set_bit(SPEAKER_DDR, SPEAKER);
    }
    else{			/* otherwise, turn output off */
      clear_bit(SPEAKER_DDR, SPEAKER);
    }
    thisStep = patternStep;
    while(thisStep == patternStep){;} /* wait until next step */
  } 
  clear_bit(LED_PORT, LED0);
}

void recordPattern(uint8_t* buffer){
  uint8_t thisStep;
  uint8_t currentNote;
  patternStep = 0;
  set_bit(TIMSK2, OCIE2A);	      /* enable T2 interrupt */
  while(patternStep < PATTERN_LEN){	/* loop until done */
    if ((patternStep & 0b00000111) == 1){ 
      toggle_bit(LED_PORT, LED0);
    }
    if (bit_is_set(UCSR0A, RXC0)){	/* just got bit */
      currentNote = notemap[UDR0-' '];
      buffer[patternStep] = currentNote;
      OCR0A = currentNote; 	/* play the note just received */
      set_bit(SPEAKER_DDR, SPEAKER);
      thisStep = patternStep;
      while(thisStep == patternStep){;} /* wait until next step */
      clear_bit(SPEAKER_DDR, SPEAKER);  /* off at end of note */
    }
    else{
      buffer[patternStep] = 0;
    }
  }
  clear_bit(LED_PORT, LED0);
}

void clickTrack(void){
  uint8_t thisStep;
  set_bit(LED_PORT, LED0);
  patternStep = 0;
  set_bit(TIMSK2, OCIE2A);	      /* enable T2 interrupt */
  while(patternStep < PATTERN_LEN/2){	/* loop until done */
    toggle_bit(LED_PORT, LED0);
    if ((patternStep & 0b00001111) == 1){ /* only click every 16th sample */
      OCR0A = 50;
      set_bit(SPEAKER_DDR, SPEAKER);
    }
    thisStep = patternStep;
    while(thisStep == patternStep){;} /* wait until next step */
    clear_bit(SPEAKER_DDR, SPEAKER);  /* off at end of note */
    
  }
  clear_bit(LED_PORT, LED0);
}

void freePlay(void){
  uint8_t currentNote;
  uint8_t thisStep;
  set_bit(LED_PORT, LED0);
  patternStep = 0;
  set_bit(TIMSK2, OCIE2A);	      /* enable T2 interrupt */
  while(patternStep < PATTERN_LEN){	/* loop until done */
    toggle_bit(LED_PORT, LED0);
    if (bit_is_set(UCSR0A, RXC0)){	/* just got bit */
      currentNote = UDR0;
      if (currentNote == 'q'){
	break;
      }
      currentNote = notemap[currentNote -' '];
      OCR0A = currentNote; 	/* play the note just received */
      set_bit(SPEAKER_DDR, SPEAKER);
      
      thisStep = patternStep;
      while(thisStep == patternStep){;} /* wait until next step */
      clear_bit(SPEAKER_DDR, SPEAKER);  /* off at end of note */
    }
  }
  clear_bit(LED_PORT, LED0);  
}

void initNotemap(void){
    uint8_t i;
  for (i=0; i<96; i++){
    notemap[i] = 0;
  }
  notemap['a'-' '] = G1;
  notemap['s'-' '] = A1;
  notemap['d'-' '] = B1;
  notemap['f'-' '] = C2;
  notemap['g'-' '] = D2;
  notemap['h'-' '] = E2;
  notemap['j'-' '] = F2;
  notemap['k'-' '] = G2;
  notemap['l'-' '] = A2;
  notemap[';'-' '] = B2;
  notemap['\''-' '] = C3;
}

int main(void){

  // -------- Inits --------- //
  uint8_t recordBuffer[PATTERN_LEN];
  char input1;
  char input2;
  uint8_t* patternArray[4] = {pat1, pat2, pat3, pat4};
  initNotemap();
  initTimer0();
  initTimer2();
  sei();
  initUART();
  set_bit(LED_DDR, LED0);
  
  while(1){
    
    printString("\r\n------  Playback / Recorder  ------\r\n");
    printString(" [r] to record a track\r\n");
    printString(" [1-4] to play back a track\r\n");
    printString(" [q] to toggle free play\r\n");
    input1 = receiveByte();
    switch(input1){
    case 'q':  
      freePlay();
      break;
    case 'r':
      clickTrack();
      recordPattern(recordBuffer);
      do{
	printString("\r\n[r]eplay, save to EEPROM [1-4], or discard? ");
	input2 = receiveByte();
	if (input2 >= '1' && input2 <= '4'){
	  eeprom_update_block(recordBuffer, patternArray[input2-'1'], PATTERN_LEN);
	}
	if (input2 == 'r'){
	  playBuffer(recordBuffer);
	}
      } while(input2 == 'r');
      break;
    case '1':
      playPattern(patternArray[0]);
      break;
    case '2':
      playPattern(patternArray[1]);
      break;
    case '3':
      playPattern(patternArray[2]);
      break;
    case '4':
      playPattern(patternArray[3]);
      break;
      
    default:
      break;
    }
    
  }   
  // ------ Event loop ------ //
  return(0);                  /* This line is never reached  */
}

