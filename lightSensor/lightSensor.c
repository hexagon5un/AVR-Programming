// Quick Demo of light sensor 

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "USART.h"


// -------- Functions --------- //
static inline void initADC0(void){
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    /* ADC clock prescaler /32 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
  ADCSRA |= (1 << ADSC);                    /* start warmup conversion */
  ADMUX |= (1 << ADLAR);     /* right-adjust bits, only 8 bits in ADCH */
  loop_until_bit_is_clear(ADCSRA, ADSC);            /* wait until done */
}

int main(void){

  // -------- Inits --------- //
  uint8_t ledValue;
  uint8_t i;

  initADC0();
  initUSART();
  LED_DDR = 0xff;

  // ------ Event loop ------ //
  while(1){
                                                  /* Read in ADC value */
    ADCSRA |= (1 << ADSC);                     /* start ADC conversion */
    loop_until_bit_is_clear(ADCSRA, ADSC);          /* wait until done */
    transmitByte(ADCH);
                                                    /* Display on LEDs */
                         /* Have 8 bits, want 3 (eight LEDs after all) */
    ledValue = ADCH >> 5;
    LED_PORT = 0;
                                   /* Light up all LEDs up to ledValue */
    for (i=0; i <= ledValue; i++){
      LED_PORT |= (1 << i);
    }
    _delay_ms(50);
  }                                                  /* End event loop */
  return(0);                             /* This line is never reached */
}

