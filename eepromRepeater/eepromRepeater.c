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

uint8_t  notemap[96];


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
    toggle_bit(LED_PORT, LED0);
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

void recordPattern(uint8_t* buffer){
  uint8_t thisStep;
  uint8_t currentNote;
  patternStep = 0;
  set_bit(TIMSK2, OCIE2A);	      /* enable T2 interrupt */
  while(patternStep < PATTERN_LEN){	/* loop until done */
    toggle_bit(LED_PORT, LED0);
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
  while(patternStep < PATTERN_LEN){	/* loop until done */
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
  initNotemap();
  initTimer0();
  initTimer2();
  sei();
  initUART();
  set_bit(LED_DDR, LED0);
  
  while(1){
    
    printString("\r\n------  Playback / Recorder  ------\r\n");
    printString(" [r] + 1-4 to record a track\r\n");
    printString(" [p] + 1-4 to play a track once\r\n");
    printString(" [q] to toggle free play\r\n");
    input1 = receiveByte();
    switch(input1){
    case 'q':  
      freePlay();
      break;
    case 'r':
      printString("Select a track [1-4]: ");
      input2 = receiveByte();
      clickTrack();
      recordPattern(recordBuffer);
      switch(input2){
      case '1':
	eeprom_update_block(recordBuffer, pat1, PATTERN_LEN);
	break;
      case '2':
	eeprom_update_block(recordBuffer, pat2, PATTERN_LEN);
	break;
      case '3':
	eeprom_update_block(recordBuffer, pat3, PATTERN_LEN);
	break;
      case '4':
	eeprom_update_block(recordBuffer, pat4, PATTERN_LEN);
	break;
      default:
	break;
      }
      break;
    case 'p':
      printString("Select a track [1-4]: ");
      input2 = receiveByte();
      switch(input2){
      case '1':
	playPattern(pat1);
	break;
      case '2':
	playPattern(pat2);
	break;
      case '3':
	playPattern(pat3);
	break;
      case '4':
	playPattern(pat4);
	break;
      default:
	break;
      }
      break;
    default:
      break;
    }
    
  }   
  // ------ Event loop ------ //
  return(0);                  /* This line is never reached  */
}

