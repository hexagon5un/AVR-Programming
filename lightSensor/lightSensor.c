/* Quick Demo of light sensor */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //    

// -------- Functions --------- //
static inline void initADC0(void){
  ADMUX |= (1 << REFS0);            /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS2);
  ADCSRA |= (1 << ADPS0);           /* ADC clock prescaler /32 */
  ADCSRA |= (1 << ADEN);	    /* enable ADC */
  ADCSRA |= (1 << ADSC);	    /* start warmup conversion */
  loop_until_bit_is_clear(ADCSRA, ADSC);  /* wait until done */
}

static inline void transmit16Bits(uint16_t sixteenBitNumber){
  /* Synchronization code */
  printString("hello");
  /* Our data */
  /* Send most-significant byte*/
  transmitByte((uint8_t) (sixteenBitNumber >> 8));
  /* Send least-significant byte */
  transmitByte(sixteenBitNumber);
}


int main(void){

  // -------- Inits --------- //
  uint8_t ledValue;
  uint8_t i;

  initADC0();
  initUSART();
  LED_DDR = 0xff;
  _delay_ms(1000);

  // ------ Event loop ------ //
  while(1){     
    
    /* Read in ADC value */
    ADCSRA |= (1 << ADSC);		   /* start ADC conversion */
    loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */

    transmit16Bits(ADC);	/* send value over serial */
     
    /* Display on LEDs */
    /* Have 10 bits, want 3 (eight LEDs after all) */
    ledValue = ADC >> 7; 
    LED_PORT = 0;
    /* Light up all LEDs up to ledValue */
    for (i=0; i <= ledValue; i++){
      LED_PORT |= (1 << i);	
    }

    _delay_ms(50);
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

