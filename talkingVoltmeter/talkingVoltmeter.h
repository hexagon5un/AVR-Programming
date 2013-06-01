/* Include file with DPCM data in it */
#include "allDigits.h"
/* Now define sample-table names used in digits file */
#define ONE_TABLE    DPCM_one_8000
#define TWO_TABLE    DPCM_two_8000
#define THREE_TABLE  DPCM_three_8000
#define FOUR_TABLE   DPCM_four_8000
#define FIVE_TABLE   DPCM_five_8000
#define SIX_TABLE    DPCM_six_8000
#define SEVEN_TABLE  DPCM_seven_8000
#define EIGHT_TABLE  DPCM_eight_8000
#define NINE_TABLE   DPCM_nine_8000
#define ZERO_TABLE   DPCM_zero_8000
#define POINT_TABLE  DPCM_point_8000
#define VOLTS_TABLE  DPCM_volts_8000
#define INTRO_TABLE  DPCM_talkingvoltmeter_8000

#define SPEECH_DELAY   2000   /* milliseconds */

char     welcome[] PROGMEM = \
  "\r\n---------===(  Talking Voltmeter  )===-----------\r\n";

/* --------------- Globals used by the ISR -------------- */
volatile uint8_t* thisTableP; /* points at the current speech table */
volatile uint16_t thisTableLength; /* length of current speech table */

volatile uint16_t sampleNumber;         // sample index
volatile int8_t   out, lastout;		// output values
volatile uint8_t  differentials[4] = {0,0,0,0};	 
const    int8_t   dpcmWeights[4]   = {-12, -3, 3, 12};



/* These arrays let us choose a table (and its length) numerically */
uint8_t* tablePointers[] PROGMEM = { 
  ZERO_TABLE, ONE_TABLE, TWO_TABLE, THREE_TABLE, FOUR_TABLE, 
  FIVE_TABLE, SIX_TABLE, SEVEN_TABLE, EIGHT_TABLE, NINE_TABLE, 
  POINT_TABLE, VOLTS_TABLE, INTRO_TABLE
};
uint16_t  tableLengths[]  = {	/* all sample tables are 8-bit */
  sizeof(ZERO_TABLE), sizeof(ONE_TABLE), sizeof(TWO_TABLE), 
  sizeof(THREE_TABLE), sizeof(FOUR_TABLE), sizeof(FIVE_TABLE),
  sizeof(SIX_TABLE), sizeof(SEVEN_TABLE), sizeof(EIGHT_TABLE), 
  sizeof(NINE_TABLE), sizeof(POINT_TABLE), sizeof(VOLTS_TABLE), 
  sizeof(INTRO_TABLE)
};
void selectTable(uint8_t whichTable){
  /* Set up global table pointer, lengths */
  thisTableP = (uint8_t*) pgm_read_word(&tablePointers[whichTable]);
  thisTableLength = tableLengths[whichTable];
}


/* These provide macro-like correspondence to the tables */
enum {
   ZERO, ONE, TWO, THREE, FOUR, 
  FIVE, SIX, SEVEN, EIGHT, NINE, 
  POINT, VOLTS, INTRO
};

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
  // Timer 2 loads OCR0A, provides sampling frequency
  TCCR2A = (1<<WGM21);	      /* CTC, count to OCR2A */
  TIMSK2 = (1<<OCIE2A);	      /* turn on compare interrupt */
  /* note: no clock source selected yet, so won't start up  */
}

void initADC(void){
  // ADC for Voltmeter function
  ADMUX  |= (0b00001111 & PC5);  /* set mux to ADC5 */
  DIDR0 |= _BV(ADC5D);		 /* turn off digital circuitry on PC5 */
  ADMUX  |= (1 << REFS0);        /* reference voltage is AVCC, 5V */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);        /* enable ADC */
}
