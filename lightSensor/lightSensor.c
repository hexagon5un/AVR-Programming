/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

// -------- Global Variables --------- //    

// -------- Functions --------- //
static inline void initTemperature(void){
  ADMUX |= 0;                   /* set to ADC0 */
  ADMUX |= (1 << REFS0); /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN) | (1 << ADSC); /* enable and start dummy conversion */
  loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */
}

int main(void){

  // -------- Inits --------- //
  uint8_t i;
  uint16_t onValue;
  uint16_t offValue;

  initTemperature();
  set_bit(LED_DDR, LED0);
  initUSART();
  _delay_ms(200);

  // ------ Event loop ------ //
  while(1){     
    
    
    onValue = 0;
    offValue = 0;

    set_bit(LED_PORT, LED0);
    _delay_ms(5);      
    for (i=0; i<64; i++){
      ADCSRA |= (1 << ADSC);
      loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */
      onValue += ADC;
    }

    clear_bit(LED_PORT, LED0);
    _delay_ms(5);      
    for (i=0; i<64; i++){
      ADCSRA |= (1 << ADSC);
      loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */
      offValue += ADC;
    }

    onValue = onValue >> 4;
    offValue = offValue >> 4;
    
    if (onValue >= offValue){
      transmitByte((uint8_t) onValue - offValue); 
    }
    
    

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

