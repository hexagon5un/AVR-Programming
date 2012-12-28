/* 
   Direct-digital synthesis
   ADSR Demo
   
*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include <avr/interrupt.h>	
#include "pinDefines.h"
#include "macros.h"
#include "scale.h"
#include "fullSaw7.h"
#include "USART.h"

#define FULL_VOLUME     31 	/* 5-bit volumes */

// Volume envelope default values (slightly percussive)
#define ATTACK_RATE    8
#define DECAY_RATE     50
#define SUSTAIN_LEVEL  20
#define SUSTAIN_TIME   2000
#define RELEASE_RATE   200

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
}

static inline void initLEDs(void){
  uint8_t i;
  LED_DDR = 0xff;	/* All LEDs for diagnostics */
  for (i=0; i<8; i++){
    set_bit(LED_PORT, i);
    _delay_ms(100);
    clear_bit(LED_PORT, i);
  }
}

static inline void printByte(uint8_t byte){
  uint8_t tens;
  tens = byte / 10;
  transmitByte( ((tens/10) % 10) + '0');
  transmitByte( (tens % 10) + '0');
  transmitByte( (byte % 10) + '0');
}



int main(void){

  // -------- Inits --------- //

  uint16_t accumulator = 0;  
  uint8_t  volume = 0;
  uint16_t clock  = 0;
  uint16_t tuningWord = C1;    
  uint8_t waveStep;
  int16_t mixer;
  uint8_t i;
  uint8_t parametersChanged = 1;

  // Initialize envelope parameters to default
  uint8_t attackRate = ATTACK_RATE;
  uint8_t decayRate =  DECAY_RATE;    
  uint8_t sustainLevel = SUSTAIN_LEVEL;
  uint16_t sustainTime = SUSTAIN_TIME;
  uint16_t releaseRate = RELEASE_RATE;

  uint16_t attackTime = attackRate * FULL_VOLUME;
  uint16_t decayTime = (attackTime + (FULL_VOLUME-sustainLevel) * DECAY_RATE);
  
  initTimer0();
  initUSART();
  transmitString("  Serial Synth\r\n");
  transmitString("Notes: asdfghjkl;'\r\n");
  transmitString("(-|+)      parameter: \r\n");
  transmitString("\\ resets to defaults: \r\n");

  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  // ------ Event loop ------ //
  while(1){		       

    // Set PWM output
    loop_until_bit_is_set(TIFR0, TOV0); /* wait for timer0 overflow */
    OCR0A = 128 + (uint8_t) mixer; 		
    set_bit(TIFR0, TOV0);		/* reset the overflow bit */

    // Update the DDS 
    accumulator += tuningWord;
    waveStep = accumulator >> 8;
    mixer = fullSaw7[waveStep] * volume;
    mixer = mixer >> 5;
    
    /* Dynamic Volume stuff here */
    if (clock){		     /* if clock already running */
      clock++;
      if (clock < attackTime) { /* attack */
	if (clock > attackRate*volume){ /* wait until time to increase next step */
	  if (volume < 31){
	    volume++;
	  }
	}
      }
      else if (clock < decayTime) {  /* decay */
	if ((clock - attackTime) > (FULL_VOLUME-volume)*decayRate){
	  if (volume > sustainLevel){
	    volume--;
	  }
	}
      }
      else if (clock > (decayTime + sustainTime)){  /* release  */
	if ((clock - (decayTime + sustainTime)) > (sustainLevel-volume)*releaseRate){
	  if (volume > 0){
	    volume--;
	  }
	  else{
	    clock = 0;
	  }
	}
      }
    }
    else {		       
      /* All Input processed here -- not in clock loop, check USART */

      i = receiveByte();

      switch(i){
      case 'a':
	tuningWord = G1;
	break;
      case 's':
	tuningWord = A1;
	break;
      case 'd':
	tuningWord = B1;
	break;
      case 'f':
	tuningWord = C2;
	break;
      case 'g':
	tuningWord = D2;
	break;
      case 'h':
	tuningWord = E2;
	break;
      case 'j':
	tuningWord = F2;
	break;
      case 'k':
	tuningWord = G2;
	break;
      case 'l':
	tuningWord = A2;
	break;
      case ';':
	tuningWord = B2;
	break;
      case '\'':
	tuningWord = C3;
	break;
	
	// Change parameters
      case 'q':
	if (attackRate > 2)
	  attackRate-=2;
	parametersChanged=1;
	break;
      case 'w':
	if (attackRate < 250)
	  attackRate+=2;
	parametersChanged=1;
	break;
      case 'e':
	if (decayRate > 5)
	  decayRate-=5;
	parametersChanged=1;
	break;
      case 'r':
	if (decayRate < 250)
	  decayRate+=5;
	parametersChanged=1;
	break;
      case 't':
	if (sustainLevel > 1)
	  sustainLevel--;
	parametersChanged=1;
	break;
      case 'y':
	parametersChanged=1;
	if (sustainLevel < 31)
	  sustainLevel++;
	break;
      case 'u':
	parametersChanged=1;
	if (sustainTime > 512 )
	  sustainTime-=512;
	break;
      case 'i':
	parametersChanged=1;
	if (sustainTime < 100*512)
	  sustainTime+=512;
	break;
      case 'o':
	if (releaseRate > 8)
	  parametersChanged=1;
	releaseRate -= 8;
	break;
      case 'p':
	parametersChanged=1;
	if (releaseRate < 250*8)
	  releaseRate += 8;
	break;
       
      case '\\':		/* reset to defaults */
	attackRate = ATTACK_RATE;
	decayRate =  DECAY_RATE;    
	sustainLevel = SUSTAIN_LEVEL;
	sustainTime = SUSTAIN_TIME;
	releaseRate = RELEASE_RATE;
	parametersChanged=1;

      default:
	/* If press something else display status & notes */
	parametersChanged=1;
	
      }	/* end switch */

      if (parametersChanged){ 
	// Print out status if changed
	if (parametersChanged){
	  transmitString("-------------------------\r\n");
	  transmitString("(q|w)         attack: ");
	  printByte(attackRate);
	  transmitString("\r\n");     
	  transmitString("(e|r)          decay: ");
	  printByte(decayRate);
	  transmitString("\r\n");     
	  transmitString("(t|y)  sustain level: ");
	  printByte(sustainLevel);
	  transmitString("\r\n");     
	  transmitString("(u|i) sustain length: ");
	  printByte(sustainTime>>8);
	  transmitString("\r\n");     
	  transmitString("(o|p)   release rate: ");
	  printByte(releaseRate>>3);	
	  transmitString("\r\n");
	  transmitString("\r\n");     
	  parametersChanged = 0;
	}    
	// And update parameters
	attackTime = attackRate * FULL_VOLUME;
	decayTime = (attackTime + (FULL_VOLUME-sustainLevel) * decayRate);
      }
      else{			
	// play note, start clock 
	clock = 1;
      }
    } /* end receive data "else" */

    
  } /* End event loop */
  return(0);		      /* This line is never reached  */
}
