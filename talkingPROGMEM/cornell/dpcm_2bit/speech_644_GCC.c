//  Talking Voltmeter Example

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "pinDefines.h"

#include "DPCM_rich_hello_world_8000.h"
#define TABLE_NAME  DPCM_rich_hello_world_8000

volatile uint16_t sampleNumber;         // sample index
volatile int8_t   out, lastout;		// output values
volatile uint8_t  p1, p2, p3, p4;	// hold 4 differentials
const int8_t      PCMvalue[4] = {-20, -4, 4, 20};

ISR (TIMER2_COMPA_vect){ 
  /* Timer 2 controls sampling speed -- 
     ISR reads new data, loads PWM output, OCR0A */
  /* Since we can decode 4 2-bit values at once, need to know where
     we are in the 4-step mini-cycle. */
  uint8_t cycle = sampleNumber & 0b00000011; 	/* our 4 steps */
  uint16_t tableEntry = sampleNumber >> 2;	/* 4 steps per table byte */
  uint8_t  packedData;				/* the new byte */
  switch(cycle){
  case 0:  // Start of the cycle, unpack next byte of samples
    if (tableEntry < sizeof(TABLE_NAME)){  
      packedData = pgm_read_byte(&TABLE_NAME[tableEntry]) ;
      p1 = (packedData>>6) & 3 ;
      p2 = (packedData>>4) & 3 ;
      p3 = (packedData>>2) & 3 ;
      p4 = (packedData & 3); 
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
  /* Update PWM audio output */
  OCR0A = out + 128;		/* re-center for 0-255 PWM */
  lastout = out;		/* update last value */
  sampleNumber++;		/* on to next sample */
  
  /* When done, turn off PWM, Timer0 */
  if (sampleNumber == 4*sizeof(TABLE_NAME)-1) {
    TCCR2B = 0;			/* disable sample-playback clock */
    OCR0A = 128;	       	/* idle at mid-voltage */
    lastout = 0;		/* start at 0 next time */
  }
} // end ISR

void initTimer0(void){
  // Timer 0 Configured for free-running PWM Audio Output
  TCCR0A |= (1<<WGM00) | (1<<WGM01); /* fast PWM mode */
  TCCR0A |= (1<<COM0A0) | (1<<COM0A1); /* output on PD6/OC0A */
  TCCR0B = (1<<CS00);		       /* fastest clock */
  OCR0A = 128 ;			       /* initialize mid-value */
  SPEAKER_DDR |= (1<<SPEAKER);	       /* output PD6 / OC0A */
}

void initTimer2(void){
  // Timer 2 loads OCR0A, defines sampling frequency
  // Aiming for around 8kHz
  TCCR2A = (1<<WGM21);	      /* CTC, count to OCR2A */
  OCR2A = 128;		      /* controls sample playback frequency */
  TCCR2B = (1<<CS21);	      /* clock source / 8 = 1MHz */
  TIMSK2 = (1<<OCIE2A);	      /* turn on compare interrupt */
  sei();
}

void initADC(void){
  // ADC for Voltmeter function
  ADMUX  |= (0b00001111 & PC5);  /* set mux to ADC5 */
  DIDR0 |= _BV(ADC5D);		 /* turn off digital circuitry on PC5 */
  ADMUX  |= (1 << REFS0);        /* reference voltage is AVCC, 5V */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);        /* enable ADC */
}

int main(void){ 
  uint8_t adcValue;
  
  initTimer0();
  initTimer2();
  initADC();

  while(1) {  
    sampleNumber = 0;		/* back to start of sample table */
    TCCR2B = (1<<CS21);		/* start loading samples */
    
    /* Wait until done speaking, then delay some more. */
    loop_until_bit_is_clear(TCCR2B, CS21);
    _delay_ms(1000);
    
  } /*  end while  */
  return(0);
}  



