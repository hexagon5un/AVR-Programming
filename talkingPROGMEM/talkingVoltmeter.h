/* Include file with DPCM data in it */
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

#define SPEECH_DELAY     2000   /* milliseconds */

char     welcome[] PROGMEM = \
  "\r\n---------===(  Talking Voltmeter  )===-----------\r\n";

/* Pointers (in progmem!) to the digits (in progmem). */
/* This lets us call up the right table: tablePointers[1] for ONE */
uint8_t* tablePointers[] PROGMEM = { 
  ZERO, ONE, TWO, THREE, FOUR, FIVE,
  SIX, SEVEN, EIGHT, NINE, POINT, VOLTS
};
uint16_t  tableLengths[]  = {
  sizeof(ZERO), sizeof(ONE), sizeof(TWO), 
  sizeof(THREE), sizeof(FOUR), sizeof(FIVE),
  sizeof(SIX), sizeof(SEVEN), sizeof(EIGHT), 
  sizeof(NINE), sizeof(POINT), sizeof(VOLTS)
};

/* Globals used by the ISR */
volatile uint8_t  whichDigit = 0; 
volatile uint16_t sampleNumber;         // sample index
volatile int8_t   out, lastout;		// output values
volatile uint8_t  p1, p2, p3, p4;	// hold 4 differentials
int8_t      PCMvalue[4] = {-18, -4, 4, 18};


///-----------------   Init functions  -------------------///

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
