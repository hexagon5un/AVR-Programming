                                             /* POV toy demo framework */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>

#define DELAYTIME 2                                              /* ms */
#define LED_PORT                PORTB
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

// -------- Functions --------- //
void POVDisplay(uint8_t povData[], uint8_t numberRows) {
  uint8_t i;
  for (i = 0; i < numberRows; ++i) {
    LED_PORT = povData[i];
    _delay_ms(DELAYTIME);
  }
  LED_PORT = 0;
  _delay_ms(5 * DELAYTIME);
}

int main(void) {
  // -------- Inits --------- //
  LED_DDR = 0xff;                 /* Set up all of LED pins for output */
  // ------ Event loop ------ //
  while (1) {                                              /* mainloop */
    POVDisplay(invaderData1, sizeof(invaderData1));
    _delay_ms(5 * DELAYTIME);
    POVDisplay(invaderData2, sizeof(invaderData2));
    _delay_ms(5 * DELAYTIME);
  }                                                    /* end mainloop */
  return (0);
}
