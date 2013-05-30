//  Talking Voltmeter Example

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "pinDefines.h"
#include "USART.h"

#include "allDigits.h"
/* Now define sample-table names used in digits file */
#define ONE  DPCM_one_8000
#define TWO  DPCM_two_8000
#define THREE  DPCM_three_8000
#define FOUR  DPCM_four_8000
#define FIVE  DPCM_five_8000
#define SIX  DPCM_six_8000
#define SEVEN  DPCM_seven_8000
#define EIGHT  DPCM_eight_8000
#define NINE  DPCM_nine_8000
#define ZERO  DPCM_zero_8000
#define POINT  DPCM_point_8000
#define VOLTS  DPCM_volts_8000

const char     welcome[] PROGMEM = "\r\n---------===(  Talking Voltmeter  )===-----------\r\n";
const uint8_t* tablePointers[] PROGMEM = {ZERO, ONE, TWO, THREE, FOUR, FIVE,
					  SIX, SEVEN, EIGHT, NINE, POINT, VOLTS};
const uint16_t  tableLengths[]  = {sizeof(ZERO), sizeof(ONE), sizeof(TWO), 
			     sizeof(THREE), sizeof(FOUR), sizeof(FIVE),
			     sizeof(SIX), sizeof(SEVEN), sizeof(EIGHT), 
			     sizeof(NINE), sizeof(POINT), sizeof(VOLTS)};
volatile uint8_t  whichDigit = 0; 
volatile uint16_t sampleNumber;         // sample index
volatile int8_t   out, lastout;		// output values
volatile uint8_t  p1, p2, p3, p4;	// hold 4 differentials
const int8_t      PCMvalue[4] = {-18, -4, 4, 18};

ISR (TIMER2_COMPA_vect){ 
  /* Timer 2 controls sampling speed -- 
     ISR reads new data, loads PWM output, OCR0A */
  /* Since we can decode 4 2-bit values at once, need to know where
     we are in the 4-step mini-cycle. */
  uint8_t cycle = sampleNumber & 0b00000011;    /* keep last 2 bits */
  uint16_t tableEntry = sampleNumber >> 2;	/* 4 steps per table byte */
  uint8_t  packedData;				/* the new byte */
  const uint8_t* thisTableP;

  switch(cycle){
  case 0:  // Start of the cycle, unpack next byte of samples
    if (tableEntry < tableLengths[whichDigit]){  
      thisTableP = (const uint8_t*) pgm_read_word(&tablePointers[whichDigit]);
      packedData = pgm_read_byte(&thisTableP[tableEntry]) ;
      p1 = (packedData>>6) & 0b00000011 ; /* mask all but last 2 bits */
      p2 = (packedData>>4) & 0b00000011 ;
      p3 = (packedData>>2) & 0b00000011 ;
      p4 = (packedData     & 0b00000011); 
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
  if (sampleNumber == 4*tableLengths[whichDigit]-1) {
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

static inline void speakDigit(void){
  sampleNumber = 0;		/* back to start of sample table */
  TCCR2B = (1<<CS21);		/* start loading samples */
  /* Wait until done speaking */
  loop_until_bit_is_clear(TCCR2B, CS21);
}


int main(void){ 
  uint16_t voltage;
  uint8_t volts;
  uint8_t tenths;
  uint8_t vcc = 50;
  
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

    voltage = 10*ADC*vcc + vcc/2; 
    voltage = voltage >> 10;   
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
    
    _delay_ms(2000);
    
  } /*  end while  */
  return(0);
}  



