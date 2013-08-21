// Volume is set to zero when it falls below this threshold
#define SILENCE_THRESHOLD    160
#define VOLUME_SCALE         3
// Combined with your LDR sensitivity, this determines the lowest pitch
#define MAX_TUNING_WORD      2100UL

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

