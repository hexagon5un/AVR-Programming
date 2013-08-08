/* 
Using two light sensors (pitch and volume),
and our previous PWM DDS audio routines,
plus a whole ton of ISRs,
we can make a fairly decent-sounding Theremin-like instrument.
*/

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "fullTri7.h"

#define MAX_TUNING_WORD  2100UL
/* Combined with your LDR sensitivity, this determines the pitch range */
#define GATE_THRESHOLD 5 
/* Volume is set to zero when it falls below this threshold */

// -------- Global Variables --------- //    

volatile uint16_t accumulator;  
volatile uint16_t tuningWord;
volatile uint8_t  volume;	
volatile uint16_t volume_ADC_average;	
volatile uint16_t tuning_ADC_average;	
volatile uint16_t volume_ADC_max;  /* simple autocalibration */

// -------- Functions --------- //

static inline void initTimer0(void){
  /* Fast PWM mode, output on OCR0A */
  TCCR0A |= (BV(WGM00) | BV(WGM01) | BV(COM0A1)); 
  TCCR0B |= (1 << CS00);	 /* Clock with /1 prescaler */
  TIMSK0 |= (1 << TOIE0);	 /* Overflow interrupt */
}

ISR(TIMER0_OVF_vect){
  int8_t mixer;
  /* lookup and scale by volume */
  mixer = fullTri7[(uint8_t) (accumulator >> 8)];
  mixer = ((mixer * volume) >> 8);
  OCR0A = 128 + mixer;		/* recenter output */
  accumulator += tuningWord;	/* take tuningWord steps forward */
}


static inline void initADC(void){
  ADMUX |= (1 << REFS0);               /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); /* ADC clock prescaler /32 */
  ADCSRA |= (1 << ADEN);	          /* enable ADC */
  ADCSRA |= (1 << ADIE);	          /* enable ADC-complete interrupt*/
}

ISR(ADC_vect){
  if (bit_is_set(ADMUX, MUX0)){ /* if sampling ADC1 */
    /* Calculate volume moving average */
    volume_ADC_average = (7*volume_ADC_average + ADC + 4) >> 3;
    /* in case it gets brighter in the room, adapt */
    if (volume_ADC_average > volume_ADC_max){
      volume_ADC_max = volume_ADC_average;
    }
    /* Calculate, set volume.  Shifting to get from 10-bit to 8-bit */
    volume = (volume_ADC_max - volume_ADC_average) >> 2 ;
    /* noise gate -- turn off when quiet */
    if (volume < GATE_THRESHOLD){		
      volume = 0;
    }
    /* sample on ADC0 next time */
    ADMUX &= ~(1 << MUX0); 	
    ADCSRA |= (1 << ADSC);      
  }
  else{				/* if sampling ADC0 */
    /* Again, smooth out the ADC */
   //     tuning_ADC_average = (7*tuning_ADC_average + ADC + 4) >> 3;
    tuning_ADC_average = (7*tuning_ADC_average + ADC + 4) >> 3;
    /* Bit shift multiplies the tuning sensitivity by two */
    tuningWord = MAX_TUNING_WORD - (tuning_ADC_average << 1);
    ADMUX |= (1 << MUX0); 	/* sample on ADC1 next time */
    ADCSRA |= (1 << ADSC);	/* start conversion */
  }
}

int main(void){
  // -------- Inits --------- //
  initTimer0();
  SPEAKER_DDR |= (1 << SPEAKER); /* enable output to speaker */

  initADC();
  sei();			 /* Enable all interrupts */
  ADCSRA |= (1 << ADSC);	 /* start conversions */
 
  // ------ Event loop ------ //
  while(1){     
    /* 
       empty event loop 
       with comment haiku
       invites further coding
    */
  }    
  return(0);                  
}

