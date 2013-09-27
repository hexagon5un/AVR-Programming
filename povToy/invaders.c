/*
  Space Invaders POV demo by Elliot Williams
*/

#include <avr/io.h>
#include <util/delay.h>
#define DELAYTIME 2                                              /* ms */
#include "povRoutines.h"

#define BUTTON_DDR    DDRD                            /* button on PD4 */
#define BUTTON_INPUT  PIND
#define BUTTON_PORT   PORTD
#define BUTTON        PD2

#define LED_PORT                PORTB
#define LED_PIN                 PINB
#define LED_DDR                 DDRB


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



void init(void) {
  // Set up all of bank B pins for output
  LED_DDR = 0xff;

  // Init button input, with pullup resistor
                       /* not necessary, but double-sure in input mode */
  BUTTON_DDR &= ~(1 << BUTTON);
  BUTTON_PORT |= (1 << BUTTON);                     /* activate pullup */
}


int main(void) {

  init();

  while (1) {                                              /* mainloop */

    if (bit_is_set(BUTTON_INPUT, BUTTON)) {
      POVDisplay(invaderData1, sizeof(invaderData1));
      pause();
    }

    else {
      POVDisplay(invaderData2, sizeof(invaderData2));
      pause();
    }

  }                                                    /* end mainloop */
  return (0);
}
