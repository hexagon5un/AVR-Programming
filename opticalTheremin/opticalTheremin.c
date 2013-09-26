/*
 *Using two light sensors (pitch and volume),
 *and our previous PWM DDS audio routines,
 *plus a whole ton of ISRs,
 *we can make a fairly decent-sounding Theremin-like instrument.
 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "fullTri7.h"

// Volume is set to zero when it falls below this threshold
#define SILENCE_THRESHOLD    160
#define VOLUME_SCALE         3
// Combined with your LDR sensitivity, this determines the lowest pitch
#define MAX_TUNING_WORD      2100UL

// -------- Global Variables --------- //

volatile uint16_t accumulator;
volatile uint16_t tuningWord;
volatile uint8_t volume;

// -------- Functions --------- //

static inline void initTimer0(void) {
                                     /* Fast PWM mode, output on OCR0A */
  TCCR0A |= ((1 << WGM00) | (1 << WGM01) | (1 << COM0A1));
  TCCR0B |= (1 << CS00);                    /* Clock with /1 prescaler */
  TIMSK0 |= (1 << TOIE0);                        /* Overflow interrupt */
}

ISR(TIMER0_OVF_vect) {
  int8_t mixer;
  mixer = fullTri7[(uint8_t) (accumulator >> 8)];        /* lookup DDS */
  mixer = ((mixer * volume) >> 8);   /* multiply by volume and rescale */
  OCR0A = 128 + mixer;                              /* recenter output */
  accumulator += tuningWord;          /* take tuningWord steps forward */
}

static inline void initADC(void) {
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    /* ADC clock prescaler /32 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
  ADCSRA |= (1 << ADIE);              /* enable ADC-complete interrupt */
}

ISR(ADC_vect) {                 /* called when ADC finished conversion */
  if (bit_is_set(ADMUX, MUX0)) {                  /* just sampled ADC1 */
    volume = ADC >> 2;                       /* 10-bit to 8-bit sample */
    if (volume < SILENCE_THRESHOLD) {
      volume = 0;
    }
    else {
      volume = VOLUME_SCALE * (volume - SILENCE_THRESHOLD);
    }
    ADMUX = (0b11110000 & ADMUX) | PC0;       /* sample ADC0 next time */
    ADCSRA |= (1 << ADSC);                    /* start next conversion */
  }
  else {                                          /* just sampled ADC0 */
    tuningWord = MAX_TUNING_WORD - (ADC << 1);            /* set pitch */
    ADMUX = (0b11110000 & ADMUX) | PC1;    /* sample on ADC1 next time */
    ADCSRA |= (1 << ADSC);                    /* start next conversion */
  }
}

int main(void) {

  // -------- Inits --------- //

  initTimer0();
  SPEAKER_DDR |= (1 << SPEAKER);           /* enable output to speaker */

  initADC();
  sei();                                      /* Enable all interrupts */
  ADCSRA |= (1 << ADSC);                          /* start conversions */

  // ------ Event loop ------ //

  while (1) {
    /*
     * empty event loop
     * with comment haiku
     * invites further coding
     */
  }
  return (0);
}
