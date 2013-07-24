/* Showing off some patterns to practice our bit-twiddling */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <util/delay.h>  

#define DELAYTIME 85		/* milliseconds */
#define LED_PORT                PORTB
#define LED_DDR                 DDRB

void setBit(uint8_t bit){
  LED_PORT |= (1 << bit);	/* turn on the bit'th pin */
  _delay_ms(DELAYTIME);		/* wait */
}

void clearBit(uint8_t bit){
  LED_PORT &= ~(1 << bit);	/* turn on the bit'th pin */
  _delay_ms(DELAYTIME);		/* wait */
}

void goLeft(void){
  uint8_t i;
  for (i=0; i<8; i++){
    setBit(i);
  }
  for (i=0; i<8; i++){
    clearBit(i);
  }
}

void goRight(void){
  uint8_t i;
  for (i=7; i<255; i--){
    setBit(i);
  }
  for (i=7; i<255; i--){
    clearBit(i);
  }
}

void animateByte(uint8_t byte){
  LED_PORT |= byte;		/* display byte */
  _delay_ms(DELAYTIME);		/* wait */
  LED_PORT &= ~byte;		/* then clear it */
}

void centerWave(void){
  uint8_t i;
  for (i=0; i<3; i++){
    animateByte( 1 << (4+i) | 1 << (3-i) );
  }
  for (i=0; i<3; i++){
    animateByte( 1 << (7-i) | 1 << (0+i) );
  }
}

void cylonEyes(void){
  uint8_t i;
  for (i=0; i<7; i++){
    animateByte(1 << i);
  }
  for (i=7; i>0; i--){
    animateByte(1 << i);
  }
}


int main(void){
  
  uint8_t repetitions;
  uint16_t randomNumber = 1234;
  uint8_t whichLED; 
  // -------- Inits --------- //
  LED_DDR = 0xff;    		/* all LEDs configured for output */
  // ------ Event loop ------ //
  while(1){     

    /* Init all off */
    LED_PORT = 0;

    for (repetitions=0; repetitions < 5; repetitions++){
      cylonEyes();
    }

    goLeft();
    _delay_ms(5*DELAYTIME);
   
    goRight();
    _delay_ms(5*DELAYTIME);

    for (repetitions=0; repetitions < 5; repetitions++){
      centerWave();
    }

    LED_PORT = 0;
    _delay_ms(5*DELAYTIME);   
    /* Toggle at random */
    for (repetitions=0; repetitions < 50; repetitions++){
      randomNumber = 2053*randomNumber + 13849;
      whichLED = (randomNumber >> 8) & 0b00000111;
      LED_PORT ^= (1 << whichLED); /* toggle one bit */
      _delay_ms(DELAYTIME);
    }

  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

