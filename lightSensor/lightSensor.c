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
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);  /* ADC clock prescaler /32 */
  ADCSRA |= (1 << ADEN);		  /* enable ADC */
  ADCSRA |= (1 << ADSC);	/* start first (warmup) conversion */
  loop_until_bit_is_clear(ADCSRA, ADSC);  /* wait until done */
}

int main(void){

  // -------- Inits --------- //
  uint16_t lightValue;
  uint8_t ledValue;
  uint8_t i;

  initADC0();
  initUSART();
  LED_DDR = 0xff;
  _delay_ms(1000);

  // ------ Event loop ------ //
  while(1){     
    
    ADCSRA |= (1 << ADSC);		   /* start ADC conversion */
    loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */
    lightValue = ADC;			   /* read value out */

    /* Synchronization code */
    transmitByte(0xA5);
    transmitByte(0x5A);

    /* Our data */
    transmitByte((uint8_t) (lightValue >> 8));
    transmitByte(lightValue);
     
    /* Display on LEDs */
    /* Have 10 bits, want 3 (range = 0..7) */
    ledValue = (127 + lightValue) >> 7; 
    LED_PORT = 0;
    for (i=0; i<ledValue; i++){
      LED_PORT |= (1 << i);
    }

    _delay_ms(50);
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

