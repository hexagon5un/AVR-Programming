/*
  POV demo with words
*/

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#include "thinFont.h"                       /* provides thinFont array */
#define DELAYTIME 1                           /* Time per blink, in ms */
#define DEBOUNCETIME  10

#define BUTTON                  PD2
#define BUTTON_PORT             PORTD
#define BUTTON_PIN              PIND
#define BUTTON_DDR              DDRD

#define LED_PORT                PORTB
#define LED_PIN                 PINB
#define LED_DDR                 DDRB

void POVString(char *myString) {
  uint8_t letterNum = 0;
  uint8_t column;

  do {

                                /* Display character, column-by-column */
    for (column = 0; column < 5; column++) {
      LED_PORT = thinFont[myString[letterNum] - ' '][column];
      _delay_ms(DELAYTIME);
    }

    // Delay between characters, go to next letter
    PORTB = 0;
    _delay_ms(DELAYTIME);
    _delay_ms(DELAYTIME);
    _delay_ms(DELAYTIME);
    letterNum++;

  } while (myString[letterNum]);         /* repeat until end of string */
}

uint8_t debounce(void) {
  // Just waiting a bit and re-checking.
  _delay_ms(DEBOUNCETIME);
  if (bit_is_clear(BUTTON_PIN, BUTTON)) {      /* button still pressed */
    return (1);
  }
  else {
    return (0);
  }
}

int main(void) {

  uint8_t whichPattern;
  uint8_t buttonIsPressed;
  uint8_t i;

  LED_DDR = 0xff;                                /* all LED for output */
  BUTTON_PORT |= (1 << BUTTON);                    /* pullup on button */

  while (1) {                                              /* mainloop */

    if (bit_is_clear(BUTTON_PIN, BUTTON)) {          /* button pressed */
      if (debounce()) {
        if (!buttonIsPressed) {          /* if first time it's pressed */
          whichPattern++;                        /* go to next pattern */
        }
      }
      buttonIsPressed = 1;
    }
    else {                                       /* button not pressed */
      buttonIsPressed = 0;
    }

                                      /* limit number of patterns here */
    if (whichPattern > 2) {
      whichPattern = 0;
    }

    switch (whichPattern) {
    case 0:
      // Say Hello
      POVString("Hello Demo!   ");
      break;
    case 1:
      // Cylons POV
      for (i = 0; i < 7; i++) {
        LED_PORT = (1 << i);
        _delay_ms(DELAYTIME);
      }
      for (i = 7; i > 0; i--) {
        LED_PORT = (1 << i);
        _delay_ms(DELAYTIME);
      }
      break;
    case 2:
      // Blocks
      LED_PORT = 0b11111111;
      _delay_ms(DELAYTIME);
      LED_PORT = 0b10000001;
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      LED_PORT = 0b11111111;
      _delay_ms(DELAYTIME);
      LED_PORT = 0;
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      _delay_ms(DELAYTIME);
      break;
    }

  }                                                    /* end mainloop */
  return (0);
}
