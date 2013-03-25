/* ADC Voltmeter */
/* Continuously outputs voltage over the serial line. */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/sleep.h>		/* for ADC sleep mode */

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

#define SCALEFACTOR 373UL   /* 5.05V * 3 / 4096 * 10000   */
/* Note: This voltmeter is only as accurate as your reference VCC (5V) voltage.
   The wall-wart power supply I was using would put out between 5.05 and 5.1 V,
   meaning I'd have to switch between 372 and 373.  Boo.
   The factor of three undoes the 3x voltage divider on input.
   We measure 4096 steps instead of 1024 by using 16x oversampling.
 */

// -------- Functions --------- //
static inline void initADC(void){
  ADMUX  |= (0b00001111 & PC5);  /* set mux to ADC5 */
  ADMUX  |= (1 << REFS0);        /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);        /* enable ADC */
  ADCSRA |= (1 << ADIE);	/* enable ADC interrupt */
  sei();			/* enable global interrupts */
  set_sleep_mode(SLEEP_MODE_ADC); 
}

static inline uint16_t oversample16x(void){
  uint16_t oversampledValue=0;
  uint8_t i;
  for (i=0; i<16; i++){
    sleep_mode();			   /* chip to sleep, takes ADC sample */
    oversampledValue += ADC;		   /* add them up 16x */
  }
  return(oversampledValue >> 2);           /* divide back down by four */
}

ISR(ADC_vect){
  ;		 /* doesn't do anything, just here to wake up */
}

static inline void printVoltage(uint32_t voltage);
/* Formats our voltage result for serial output */

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
   
    /* Moving average smoothing and oversampling. */
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
  /* Ten-thousands, will end up being tens of volts */
  digit = 0;
  while(voltage >= 10000){
    digit++;                    
    voltage -= 10000;
  }
  if (digit){
    transmitByte('0' + digit);
  }
  else{
    transmitByte(' ');
  }

  /* Thousands, or volts */
  digit = 0;
  while(voltage >= 1000){
    digit++;                    
    voltage -= 1000;
  }
  transmitByte('0' + digit);
  transmitByte('.');
  
  /* add up hundreds, tenths of volts */
  digit = 0;
  while(voltage >= 100){
    digit++;                    
    voltage -= 100;
  }
  transmitByte('0' + digit);
  
  /* tens */
  digit = 0;
  while(voltage >= 10){
    digit++;                    
    voltage -= 10;
  }
  transmitByte('0' + digit);
  /* ones */
  transmitByte('0' + voltage);
  transmitByte(' '); 
  transmitByte('V'); 
  transmitByte('\r');
  transmitByte('\n');
  
}
