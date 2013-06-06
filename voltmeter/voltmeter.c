/* ADC Voltmeter */
/* Continuously outputs voltage over the serial line. */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>         
#include <avr/interrupt.h>
#include <avr/sleep.h>		/* for ADC sleep mode */

#include "pinDefines.h"
#include "macros.h"
#include "UART.h"

#define SCALEFACTOR 3702UL   /* 5.053 V * 3 / 4095 * 1000000 */
/* Note: This voltmeter is only as accurate as your reference voltage.
   If you want four digits of accuracy, need to measure your AVCC well.
   If possible, plug in the measured voltage off of the AREF pin here.
   The factor of three undoes the 3x voltage divider on input.
   4095 = 14-bit max value (b/c we'll use 16x oversampling)
   1000000 is to avoid dealing with decimal points.
 */

// -------- Functions --------- //
static inline void initADC(void){
  ADMUX  |= (0b00001111 & PC5);  /* set mux to ADC5 */
  ADMUX  |= (1 << REFS0);        /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2); /* ADC clock prescaler /64 */
  ADCSRA |= (1 << ADEN);        /* enable ADC */
}

static inline void setupADCSleepmode(void){
  set_sleep_mode(SLEEP_MODE_ADC); /* defined in avr/sleep.h */
  ADCSRA |= (1 << ADIE);	  /* enable ADC interrupt */
  sei();			  /* enable global interrupts */
}

EMPTY_INTERRUPT(ADC_vect);

static inline uint16_t oversample16x(void){
  uint16_t oversampledValue=0;
  uint8_t i;
  for (i=0; i<16; i++){
    sleep_mode();			   /* chip to sleep, takes ADC sample */
    oversampledValue += ADC;		   /* add them up 16x */
  }
  return(oversampledValue >> 2);           /* divide back down by four */
}

static inline void printVoltage(uint32_t voltage);
/* Formats our voltage result for serial output */

int main(void){
  
  uint32_t avgVoltage;
  uint8_t i;
  
  // -------- Inits --------- //
  _delay_ms(100);
  initUART();
  _delay_ms(100);
  printString("\r\nDigital Voltmeter\r\n\r\n");
  initADC();  
  setupADCSleepmode();

  // ------ Event loop ------ //
  while(1){     

    /* Turns ADC values into actual volts */
    avgVoltage = 0;
    for (i=0; i<32; i++){
      avgVoltage += oversample16x();
    }
    avgVoltage = (avgVoltage * SCALEFACTOR + 16) >> 5;

    /* Pretty output for a nice serial display. */
    printVoltage(avgVoltage);	       
    _delay_ms(1000);

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}


static inline void printVoltage(uint32_t voltage){
  uint8_t digit;
  uint32_t digitsPlace = 10000000;
  
  while(digitsPlace > 1000){ 
    digit = 0;
    while(voltage >= digitsPlace){
      digit++;                    
      voltage -= digitsPlace;
    }
    if (digit){ 		/* have a number to send */
      transmitByte('0' + digit);
    }
    else{ /* handle zeros before/after decimal point */
      if (digitsPlace > 1000000){
	transmitByte(' ');
      }
      else{
	transmitByte('0');
      }
    }
    /* Where the decimal point goes */
    if (digitsPlace == 1000000){
      transmitByte('.');
    }
    digitsPlace = digitsPlace / 10;
  }  
  transmitByte(' '); 
  transmitByte('V'); 
  transmitByte('\r');
  transmitByte('\n');
}
