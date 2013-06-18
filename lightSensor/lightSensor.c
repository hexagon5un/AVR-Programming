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
  set_bit(ADMUX, REFS0);            /* reference voltage on AVCC */
  set_bit(ADCSRA, ADPS2);
  set_bit(ADCSRA, ADPS0);           /* ADC clock prescaler /32 */
  set_bit(ADCSRA, ADEN);	    /* enable ADC */
  set_bit(ADCSRA, ADSC);	    /* start warmup conversion */
  loop_until_bit_is_clear(ADCSRA, ADSC);  /* wait until done */
}

static inline void transmit16Bits(uint16_t sixteenBitNumber){
  /* Synchronization code */
  transmitByte(0xA5);
  transmitByte(0x5A);
  
  /* Our data */
  /* Send most-significant byte*/
  transmitByte((uint8_t) (sixteenBitNumber >> 8));
  /* Send most-significant byte */
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
    set_bit(ADCSRA, ADSC);		   /* start ADC conversion */
    loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */

    transmit16Bits(ADC);	/* send value over serial */
     
    /* Display on LEDs */
    /* Have 10 bits, want 3 (LED range = 0..7) */
    ledValue = ADC >> 7; 
    LED_PORT = 0;
    for (i=0; i <= ledValue; i++){
      set_bit(LED_PORT, i);	
    }

    _delay_ms(50);
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

