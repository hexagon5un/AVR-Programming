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
    
    set_bit(ADCSRA, ADSC);	   /* start ADC conversion */
    loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */
    lightValue = ADC;			   /* read value out */

    /* Synchronization code */
    transmitByte(0xA5);
    transmitByte(0x5A);

    /* Our data */
    /* Send most-significant byte*/
    transmitByte((uint8_t) (lightValue >> 8));
    /* Send most-significant byte */
    transmitByte(lightValue);
     
    /* Display on LEDs */
    /* Have 10 bits, want 3 (LED range = 0..7) */
    ledValue = lightValue >> 7; 
    LED_PORT = 0;
    for (i=0; i <= ledValue; i++){
      set_bit(LED_PORT, i);	
    }

    _delay_ms(50);
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

