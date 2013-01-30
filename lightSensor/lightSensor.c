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
    
    set_bit(LED_PORT, LED0);
    onValue = 0;
    for (i=0; i<16; i++){
      ADCSRA |= (1 << ADSC);
      loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */
      onValue += ADC;
      _delay_ms(1);
    }
    onValue = onValue >> 4;

    clear_bit(LED_PORT, LED0);
    offValue = 0;
    for (i=0; i<16; i++){
      ADCSRA |= (1 << ADSC);
      loop_until_bit_is_clear(ADCSRA, ADSC); /* wait until done */
      offValue += ADC;
      _delay_ms(1);
    }
    offValue = offValue >> 4;

    //transmitByte((uint8_t) (adcValue >> 8));
    //transmitByte((uint8_t) adcValue); 
    transmitByte((uint8_t) onValue - offValue); 

    _delay_ms(30);

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

