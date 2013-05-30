//  Talking Voltmeter Example

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "pinDefines.h"
#include "USART.h"

#include "talkingVoltmeter.h"

void unpackByte(uint8_t dataByte){
  /* Select pairs of bits from byte, save out */
  p1 = (dataByte>>6) & 0b00000011 ; 
  p2 = (dataByte>>4) & 0b00000011 ;
  p3 = (dataByte>>2) & 0b00000011 ;
  p4 = (dataByte     & 0b00000011); 
}
void updatePWMAudio(void){
  /* Update PWM audio output */
  OCR0A = out + 128;		/* re-center for 0-255 PWM */
  lastout = out;		/* update last value */
  sampleNumber++;		/* on to next sample */
}
void startSampleTimer(void){
  sampleNumber = 0;		/* back to start of sample table */
  TCCR2B = (1<<CS21);		/* start loading samples */
}
void stopSampleTimer(void){
  TCCR2B = 0;			/* disable sample-playback clock */
  OCR0A = 128;	       	/* idle at mid-voltage */
  lastout = 0;		/* start at 0 next time */
}
void speakDigit(void){
  startSampleTimer();
  /* Wait until done speaking */
  loop_until_bit_is_clear(TCCR2B, CS21);
}

ISR (TIMER2_COMPA_vect){ 
  /* Timer 2 controls sampling speed -- 
     ISR reads new data, loads PWM output, OCR0A */
  /* Since we can decode 4 2-bit values at once, need to know where
     we are in the 4-step mini-cycle. */
  uint8_t cycle = sampleNumber & 0b00000011; /* keep last 2 bits */
  uint16_t tableEntry = sampleNumber >> 2;  /* where we are in table*/
  uint8_t  packedData;			    
  uint8_t* thisTableP;

  switch(cycle){
  case 0:  // Start of the cycle, unpack next byte of samples
    if (tableEntry < tableLengths[whichDigit]){  
      thisTableP = (uint8_t*) pgm_read_word(&tablePointers[whichDigit]);
      packedData = pgm_read_byte(&thisTableP[tableEntry]) ;
      unpackByte(packedData);	/* split up byte into p1,p2,p3,p4 */
    }
    /* Add in the next PCM differential value */
    out = lastout + PCMvalue[p1] - (lastout>>3) ;  	
    break;
  case 1:
    out = lastout + PCMvalue[p2] - (lastout>>3) ;
    break;
  case 2:
    out = lastout + PCMvalue[p3] - (lastout>>3) ; 
    break;
  case 3:
    out = lastout + PCMvalue[p4] - (lastout>>3) ;
    break;
  }
  
  updatePWMAudio();

  /* When done, stop loading in new samples */
  if (sampleNumber == 4*tableLengths[whichDigit]-1) {
    stopSampleTimer();
  }
} // end ISR


int main(void){ 
  uint16_t voltage;
  uint8_t volts;
  uint8_t tenths;
  uint8_t vcc = 51; /* 10x VCC in volts (for decimal point) */  
  uint8_t i;

  initTimer0();
  initTimer2();
  initADC();
  initUSART();

  /* Print out welcome message (from PROGMEM) */
  for (i=0; i<sizeof(welcome); i++){
    transmitByte(pgm_read_byte(&welcome[i]));
  }

  while(1) {  

    ADCSRA |= (1 << ADSC);        /* start ADC */
    loop_until_bit_is_clear(ADCSRA, ADSC);

    voltage = ADC*vcc + vcc/2; 	/* extra to make rounding work */
    voltage = voltage >> 10;   	/* divide by 10-bit ADC */
    /* "voltage" is now actual 10x voltage */
    volts = voltage / 10;	
    tenths = voltage % 10;	

    whichDigit = volts;
    transmitByte('0'+volts);
    speakDigit();
    
    whichDigit = 10;  /* "point" */
    transmitByte('.');
    speakDigit();		
    
    whichDigit = tenths;
    transmitByte('0'+tenths);
    speakDigit();
   
    whichDigit = 11; /* "volts" */
    transmitByte('\r');
    transmitByte('\n');
    speakDigit();
    
    _delay_ms(SPEECH_DELAY);
    
  } /*  end while  */
  return(0);
}  



