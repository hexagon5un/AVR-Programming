//Voice decompressor example
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "USART.h"

#define TableSize 3280 //refers to the following incl file
//Contains the packed 1-bit codes for syntehsis
#include "mega644_1_bit.h" 

//reconstruction differentials 
//volatile signed char PCMvalue[4] = {-78, -16, 16, 78};
volatile signed char PCMvalue[2] = {-10, 10};

volatile unsigned int outI, tableI;     //indexes
volatile unsigned char cycle ;  		//decode phase counter
volatile signed char out, lastout, newvalue;		//output values
volatile unsigned char p1,p2,p3,p4,p5,p6,p7,p8;	//hold 8 differentials
volatile unsigned char packed	;		//byte containing 4 2-bit values	
volatile uint8_t speaking;			/* flag for whether currently speaking or not */

//generate waveform at 7812 scamples/sec
ISR (TIMER2_COMPA_vect){ 
  //compute next sample
  cycle = outI & 7;  // outI modulo 8
  if (cycle==0)      //do we need to unpack more data?
    {
      if (tableI<TableSize)  //} of stored wave?
        {
	  //unpack a table entry into 2-bit indexs
	  // pgm_read_byte (address_short) 
	  packed = pgm_read_byte(&mega644[tableI]) ;
	  //packed = DPCMAllDigits[tableI];
	  p1 = (packed & 128) == 128 ;
	  p2 = (packed & 64) == 64 ;
	  p3 = (packed & 32) == 32 ;
	  p4 = (packed & 16) == 16 ; 
	  p5 = (packed & 8) == 8 ; 
	  p6 = (packed & 4) == 4 ; 
	  p7 = (packed & 2) == 2 ; 
	  p8 = (packed & 1) == 1 ; 
	  tableI++ ; 
        } //end unpack table entry 
      //compute the output and send to PWM
      newvalue = PCMvalue[p1] ;  	
    }
  else if (cycle==1)    //don't need to unpack yet--just ouput
    newvalue = PCMvalue[p2] ; 
  else if (cycle==2)
    newvalue = PCMvalue[p3] ; 
  else if (cycle==3) 
    newvalue = PCMvalue[p4] ; 
  else if (cycle==4) 
    newvalue = PCMvalue[p5] ; 
  else if (cycle==5) 
    newvalue = PCMvalue[p6] ; 
  else if (cycle==6) 
    newvalue = PCMvalue[p7] ; 
  else if (cycle==7) 
    newvalue = PCMvalue[p8] ; 
  
  out = lastout + newvalue - (lastout>>3);
  //update outputs
  OCR0A = out + 128;
  lastout = out;
  outI++;
  //at end, turn off TCCRO
  if (tableI==TableSize) TCCR2B = 0;	  
} //ISR

int main(void){ 
  DDRD |= (1<<PD6); 
  DDRB |= (1<<PB0); 
  // turn on pwm with period= 256 cycles 
  // (62,500 samples/sec) in fast PWM mode.
  // BUT OCR0A update is done using timer2 at 7800/sec
  // timer 0 runs at full rate set in MAIN loop; TCCR0B = 1 ;
  // turn on fast PWM and OC0A output
  // 16 microsec per PWM cycle sample time
  TCCR0A = (1<<COM0A0) | (1<<COM0A1) | (1<<WGM00) | (1<<WGM01) ; 
  OCR0A = 128 ; // initialize PWM to half full scale
  TCCR0B = (1<<CS00);	 /* full speed */
  // turn on timer2 set to overflow at 7812 Hz 
  // (prescaler set to divide by 8)
  TCCR2A = (1<<WGM21);		/* CTC, count to OCR2A */
  OCR2A = 128;			/* control sample playback frequency */
  TCCR2B = (1<<CS21); 		/* clock source / 8 = 1MHz */
  TIMSK2 = (1<<OCIE2A);   // turn on compare interrupt

  sei();
  
  while(1) {  
    
    //init the output indexes
    outI = 0; 
    tableI = 0; 
    //init the ouptut value
    lastout = 0; 
    // turn on PWM
    TCCR2B = (1<<CS21);
    //wait until the speech is done then
    //time delay the next utterance.
    while (TCCR2B>0){}; 
    OCR0A = 128 ;
    _delay_ms(500);
    PORTB ^= (1<<PB0); 
    
  } // end while  
}  //end main


