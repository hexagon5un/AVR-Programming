/* 
  Space Invaders POV demo by Elliot Williams
*/

#include <inttypes.h>
#include <avr/io.h>
#define F_CPU  1000000UL
#include <util/delay.h>
#define DELAYTIME 3		/* ms */

#define BUTTON_DDR    DDRD	/* button on PD4 */
#define BUTTON_INPUT  PIND
#define BUTTON_PORT   PORTD
#define BUTTON        PD4

uint8_t invaderData1[] = {
  0b01110000,
  0b00011000,
  0b11111101,
  0b10110110,
  0b00111100,
  0b00111100,
  0b00111100,
  0b10110110,
  0b11111101,
  0b00011000,
  0b01110000
};

uint8_t invaderData2[] = {
  0b00001110,
  0b00011000,
  0b10111101,
  0b01110110,
  0b00111100,
  0b00111100,
  0b00111100,
  0b01110110,
  0b10111101,
  0b00011000,
  0b00001110
};

void invader1(void){
  for (uint8_t i = 0; i < sizeof(invaderData1) / sizeof(uint8_t); ++i) {
    PORTB = invaderData1[i];	
    _delay_ms(DELAYTIME);
  }
}


void invader2(void){
  for (uint8_t i = 0; i < sizeof(invaderData2) / sizeof(uint8_t); ++i) {
    PORTB = invaderData2[i];	
    _delay_ms(DELAYTIME);
  }
}

void pause(void){
  PORTB = 0;			/* blank for gap between repetitions */
  _delay_ms(5*DELAYTIME);
}

void init(void){
  // Set up all of bank B pins for output
  DDRB = 0xff;             

  // Init button input, with pullup resistor
  BUTTON_DDR &= ~(1<< BUTTON);   /* not necessary, but double-sure in input mode */
  BUTTON_PORT |= (1 << BUTTON);  /* activate pullup */
}


int main(void){
  
  init();

  while(1){			/* mainloop */
    
    if (bit_is_set(BUTTON_INPUT, BUTTON)){
      invader1();
      pause();
    }

    else{
      invader2();
      pause();
    }

  } /* end mainloop */
  return(0);
}
