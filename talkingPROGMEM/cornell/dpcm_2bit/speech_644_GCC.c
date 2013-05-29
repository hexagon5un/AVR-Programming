//Voice decompressor example
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//#include "DPCM_rich_8000_hello_world.h"
//#define TABLE_NAME  DPCM_rich_8000_hello_world
#include "DPCM_hiChris_8000.h"
#define TABLE_NAME  DPCM_hiChris_8000

//reconstruction differentials 
//volatile signed char PCMvalue[4] = {-78, -16, 16, 78};

volatile signed char PCMvalue[4] = {-20, -4, 4, 20};

volatile unsigned int outI, tableI;     //indexes
volatile unsigned char cycle ;  		//decode phase counter
volatile signed char out, lastout;		//output values
volatile unsigned char p1, p2, p3, p4;	//hold 4 differentials
volatile unsigned char packed	;		//byte containing 4 2-bit values	


//generate waveform at 7812 scamples/sec
ISR (TIMER2_COMPA_vect){ 
  PORTB ^= (1<<PB0); 
  //compute next sample
  cycle = outI & 3;  // outI modulo 4
  if (cycle==0)      //do we need to unpack more data?
    {
      if (tableI<sizeof(TABLE_NAME))  //end of stored wave?
        {
	  //unpack a table entry into 2-bit indexs
	  // pgm_read_byte (address_short) 
	  packed = pgm_read_byte(&TABLE_NAME[tableI]) ;
	  //packed = DPCMAllDigits[tableI];
	  p1 = (packed>>6) & 3 ;
	  p2 = (packed>>4) & 3 ;
	  p3 = (packed>>2) & 3 ;
	  p4 = (packed & 3); 
	  tableI++ ; 
        } //end unpack table entry 
      //compute the output and send to PWM
      out = lastout + PCMvalue[p1] - (lastout>>3) ;  	
    }
  else if (cycle==1)    //don't need to unpack yet--just ouput
    out = lastout + PCMvalue[p2] - (lastout>>3) ;
  else if (cycle==2)
    out = lastout + PCMvalue[p3] - (lastout>>3) ; 
  else if (cycle==3) 
    out = lastout + PCMvalue[p4] - (lastout>>3) ;
  
  //update outputs
  OCR0A = out + 128;
  lastout = out;
  outI++;
  //at end, turn off Timer0
  if (tableI==sizeof(TABLE_NAME)) {
    TCCR0B &= ~(1<<CS00);	
    OCR0A = 128;
  }
} //ISR

int main(void){ 
  uint8_t adcValue;
  DDRD |= (1<<PD6); 
  // turn on pwm with period= 256 cycles 
  // (62,500 samples/sec) in fast PWM mode.
  // BUT OCR0A update is done using timer2 at 7800/sec
  // timer 0 runs at full rate set in MAIN loop; TCCR0B = 1 ;
  // turn on fast PWM and OC0A output
  // 16 microsec per PWM cycle sample time
  TCCR0A = (1<<COM0A0) | (1<<COM0A1) | (1<<WGM00) | (1<<WGM01) ; 
  TCCR0B = (1<<CS00); 
  OCR0A = 128 ; // initialize PWM to half full scale
  
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
    TCCR0B = (1<<CS00); 
    //wait until the speech is done then
    //time delay the next utterance.
    loop_until_bit_is_clear(TCCR0B, CS00); 
    _delay_ms(1000);
       
  } // end while  
}  //end main



