// POV toy demo framework //

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>

#define DELAYTIME               2                                /* ms */
#define LED_PORT                PORTB
#define LED_DDR                 DDRB

// -------- Functions --------- //
void POVDisplay(uint8_t oneByte) {
  LED_PORT = oneByte;
  _delay_ms(DELAYTIME);
}

int main(void) {
  // -------- Inits --------- //
  LED_DDR = 0xff;                 /* Set up all of LED pins for output */
  // ------ Event loop ------ //
  while (1) {                                              /* mainloop */
    POVDisplay(0b00001110);
    POVDisplay(0b00011000);
    POVDisplay(0b10111101);
    POVDisplay(0b01110110);
    POVDisplay(0b00111100);
    POVDisplay(0b00111100);
    POVDisplay(0b00111100);
    POVDisplay(0b01110110);
    POVDisplay(0b10111101);
    POVDisplay(0b00011000);
    POVDisplay(0b00001110);

    LED_PORT = 0;
    _delay_ms(5 * DELAYTIME);
  }                                                    /* end mainloop */
  return (0);
}
