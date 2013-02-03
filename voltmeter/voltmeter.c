/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define SCALEFACTOR 372UL   /* 5.1 v * 3 / 4096 * 10000   */

// -------- Functions --------- //
static inline void initADC(void);
static inline void initADC(void){
  ADMUX |= 5;                   /* set to ADC5 */
  ADMUX |= (1 << REFS0);        /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);        /* enable ADC */
  ADCSRA |= (1 << ADSC);        /* run an intial conversion */
  loop_until_bit_is_clear(ADCSRA, ADSC); /* wait for it to finish */
}

static inline uint16_t pollADC(void); 
static inline uint16_t pollADC(void){  
    ADCSRA |= (1 << ADSC); /* ADC start conversion */
    loop_until_bit_is_clear(ADCSRA, ADSC); /* wait for it to finish */
    return(ADC);
}

static inline uint16_t oversample16x(void){
  uint16_t oversampledValue=0;
  uint8_t i;
  for (i=0; i<16; i++){
    oversampledValue += pollADC();
  }
  return(oversampledValue >> 2);
}

static inline void printVoltage(uint32_t voltage);

int main(void){

  uint16_t average_ADC;
  uint32_t voltage;
  
  // -------- Inits --------- //
  initUSART();
  transmitString("\r\nDigital Voltmeter\r\n");
  initADC();
  average_ADC = UINT16_MAX/2;

  // ------ Event loop ------ //
  while(1){     
   
    /* Moving average smoothing and oversampling.  Woot.*/
    average_ADC = (3*average_ADC + oversample16x());     
    average_ADC = average_ADC >> 2;
   
    voltage = average_ADC * SCALEFACTOR; 
    voltage = voltage / 100;

    printVoltage(voltage);
    _delay_ms(100);	       
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}


static inline void printVoltage(uint32_t voltage){
  uint8_t digit;
  
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
  
}

