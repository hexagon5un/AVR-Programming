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
  sampleNumber = 0;	 /* back to start of sample table */
  OCR2A = 128;	    /* controls sample playback frequency */
  TCCR2B = (1<<CS21);		   /* turn on timer clock */
  /* Two clock options above end up ~8kHz on 8MHz system */
}
void stopSampleTimer(void){
  TCCR2B = 0;		/* disable sample-playback clock */
  OCR0A = 128;	       	/* idle PWM at mid-voltage */
  lastout = 0;		/* start at 0 next time */
}
void speak(void){
  startSampleTimer();
  /* Wait until done speaking */
  loop_until_bit_is_clear(TCCR2B, CS21);
}
void selectTable(uint8_t whichTable){
  /* Set up global table pointer, lengths */
  thisTableP = (uint8_t*) pgm_read_word(&tablePointers[whichTable]);
  thisTableLength = tableLengths[whichTable];
}


/* Timer 2 controls sampling speed. 
   ISR reads new data, loads PWM values into OCR0A */
ISR (TIMER2_COMPA_vect){ 
  /* Since we can decode 4 2-bit values at once, need to know where
     we are in the 4-step mini-cycle. */
  uint8_t cycle = sampleNumber & 0b00000011; /* keep last 2 bits */
  uint16_t tableEntry = sampleNumber >> 2;  /* where we are in table*/
  uint8_t  packedData;			    

  switch(cycle){
  case 0:  // Start of the cycle, unpack next byte of samples
    if (tableEntry < thisTableLength){  
      /* read the next byte from the selected table */
      packedData = pgm_read_byte(&thisTableP[tableEntry]) ;
      unpackByte(packedData);	/* split up byte into p1,p2,p3,p4 */
      /* Add in the next PCM differential value */
      out = lastout + reconstruction_differentials[p1] - (lastout>>3) ;  	
    }
    else{			/* at end of table, done. */
      stopSampleTimer();
    }
    break;
  case 1:
    out = lastout + reconstruction_differentials[p2] - (lastout>>3) ;
    break;
  case 2:
    out = lastout + reconstruction_differentials[p3] - (lastout>>3) ; 
    break;
  case 3:
    out = lastout + reconstruction_differentials[p4] - (lastout>>3) ;
    break;
  }     // end  switch(cycle)
  
  updatePWMAudio();
} // end  ISR (TIMER2_COMPA_vect)


int main(void){ 
  uint16_t voltage;
  uint8_t volts;
  uint8_t tenths;
  uint8_t vcc = 51; /* 10x VCC in volts (for decimal point) */  
  uint8_t i;

  initTimer0();
  initTimer2();
  sei();		       /* for timer2 ISR */
  initADC();
  initUSART();

  /* Print out welcome message (from PROGMEM) */
  for (i=0; i<sizeof(welcome); i++){
    transmitByte(pgm_read_byte(&welcome[i]));
  }
  
  /* This is silly, but helps debug audio */
  selectTable(INTRO);  
  speak();
  
  while(1) {  

    ADCSRA |= (1 << ADSC);        /* start ADC */
    loop_until_bit_is_clear(ADCSRA, ADSC);

    voltage = ADC*vcc + vcc/2; 	/* vcc/2 to make rounding work */
    voltage = voltage >> 10;   	/* divide by 10-bits for ADC */
    /* "voltage" is now actually 10x real-world voltage */
    volts = voltage / 10;	
    tenths = voltage % 10;	
    
    selectTable(volts);	    /* 0 points to ZERO_TABLE, etc */
    transmitByte('0'+volts); 	/* serial output as well */
    speak();
    
    selectTable(POINT);  
    transmitByte('.');
    speak();		
    
    selectTable(tenths);  
    transmitByte('0'+tenths);
    speak();
   
    selectTable(VOLTS); 
    transmitByte('\r');
    transmitByte('\n');
    speak();
    
    _delay_ms(SPEECH_DELAY);
    
  } /*  end while  */
  return(0);
}  




