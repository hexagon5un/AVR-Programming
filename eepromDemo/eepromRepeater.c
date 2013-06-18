/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"
#include "scale8.h"             /* 8-bit scale */

#define PLAYBACK_PAUSE    270  	/* how long is an 8th note */
// Some globals
uint8_t  notemap[96];		   /* mapping keys to notes */

// Define EEMEM variables
#define PATTERN_LEN   16	/* 4 bars * 8th notes */
uint8_t  EEMEM pat1[PATTERN_LEN];
uint8_t  EEMEM pat2[PATTERN_LEN];
uint8_t  EEMEM pat3[PATTERN_LEN];
uint8_t  EEMEM pat4[PATTERN_LEN];

const uint8_t* patternPointer[] = {pat1, pat2, pat3, pat4}; 
/* These are pointers b/c passing arrays decays to pointers */

// -------- Functions --------- //
// Audio output on timer0
void initTimer0(void){
  TCCR0A |= (1<<WGM01) | (1<<COM0A0); /* CTC, output on pin */
  TCCR0B |= (1<<CS00) | (1<<CS01);    /* set prescaler */
}    


void playNote(uint8_t note){
  if (note){			/* play if have note */
    OCR0A = note;    
    set_bit(SPEAKER_DDR, SPEAKER);
  }
  else{				/* silence if 0 */
    clear_bit(SPEAKER_DDR, SPEAKER); 
  }
  _delay_ms(PLAYBACK_PAUSE);
  clear_bit(SPEAKER_DDR, SPEAKER); 
}

void playPattern(uint8_t* whichPattern){
  uint8_t i;
  for (i=0; i<PATTERN_LEN; i++){
    playNote(eeprom_read_byte(whichPattern + i));
  }
}

void playBuffer(uint8_t buffer[]){
  uint8_t i;
   for (i=0; i<PATTERN_LEN; i++){
     playNote(buffer[i]);
   }
}

void freePlay(void){
  char input;
  do{
    input = receiveByte();
    playNote(notemap[input - ' ']);    
  } while(!(input == 'q'));
}

void recordPattern(uint8_t buffer[]){
  uint8_t i;
  char input;
  for (i=0; i<PATTERN_LEN; i++){
    if ((i & 0b00000111) == 0){ 
      transmitByte('*');
    }
    else if ((i & 0b00000001) == 0){
      transmitByte('-');
    }
    else {
      transmitByte('.');
    }
    input = receiveByte();
    buffer[i] = notemap[input-' '];
    playNote(buffer[i]);
  }
}

void initNotemap(void){
  uint8_t i;
  for (i=0; i<96; i++){
    notemap[i] = 0;
  }
  //  notemap['a'-' '] = G1;
  notemap['s'-' '] = A0;
  notemap['d'-' '] = B0;
  notemap['f'-' '] = C1;
  notemap['g'-' '] = D1;
  notemap['h'-' '] = E1;
  notemap['j'-' '] = F1;
  notemap['k'-' '] = G1;
  notemap['l'-' '] = A1;
  notemap[';'-' '] = B1;
  notemap['\''-' '] = C2;
}

int main(void){

  // -------- Inits --------- //
  uint8_t recordBuffer[PATTERN_LEN];
  
  char input;
  initTimer0();
  initNotemap();
  initUSART();
  set_bit(LED_DDR, LED0);
  
  freePlay();
  printString("\r\n");
  recordPattern(recordBuffer);
  playBuffer(recordBuffer);

  while(1){
    
  }   
  // ------ Event loop ------ //
  return(0);                  /* This line is never reached  */
}

