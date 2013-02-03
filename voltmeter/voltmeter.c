/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define SCALEFACTOR 1465 		/* 3 * 5 / 1024  */
//#define SCALEFACTOR 1 		/* 3 * 5 / 1024  */


// -------- Functions --------- //
static inline void initADC(void){
  ADMUX |= 5;                   /* set to ADC5 */
  ADMUX |= (1 << REFS0);        /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);        /* enable ADC */
  ADCSRA |= (1 << ADSC);        /* run an intial conversion */
  loop_until_bit_is_clear(ADCSRA, ADSC); /* wait for it to finish */
}

static inline uint16_t pollADC(){  
    ADCSRA |= (1 << ADSC); /* ADC start conversion */
    loop_until_bit_is_clear(ADCSRA, ADSC); /* wait for it to finish */
    return(ADC);
}

int main(void){

  uint16_t adc_value;
  uint32_t voltage;
  uint8_t digit;
  
  // -------- Inits --------- //
  initUSART();
  transmitString("\r\nDigital Voltmeter\r\n");
  initADC();
  
  // ------ Event loop ------ //
  while(1){     
    
    adc_value = pollADC();
    voltage = adc_value * SCALEFACTOR; /*  */
    voltage = voltage / 10;

    digit = 0;
    while(voltage >= 10000){
      digit++;                    
      voltage -= 10000;
    }
    transmitByte('0' + digit);

    /* add up thousands */
    digit = 0;
    while(voltage >= 1000){
      digit++;                    
      voltage -= 1000;
    }
    transmitByte('0' + digit);
    transmitByte('.');
   
    /* add up hundreds */
    digit = 0;
    while(voltage >= 100){
      digit++;                    
      voltage -= 100;
    }
    transmitByte('0' + digit);
    
    /* add up tens */
    digit = 0;
    while(voltage >= 10){
      digit++;                    
      voltage -= 10;
    }
    transmitByte('0' + digit);
    transmitByte('0' + voltage);
    transmitByte('v');

    transmitByte('\r');
    transmitByte('\n');
    _delay_ms(500);	       
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

