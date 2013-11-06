                                    /* Demos PWM control of a DC motor */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "USART.h"

#define SPEED_STEP_DELAY 2                             /* milliseconds */

// -------- Functions --------- //
static inline void initTimer0(void) {
  TCCR0A |= (1 << WGM00);                             /* Fast PWM mode */
  TCCR0A |= (1 << WGM01);                       /* Fast PWM mode, pt.2 */
  TCCR0A |= (1 << COM0B1);                        /* output PWM to pin */
  TCCR0B |= (1 << CS02);                 /* Clock with /1024 prescaler */
  //TCCR0B |= (1 << CS00);         /* Clock with /1024 prescaler, pt.2 */
}


int main(void) {

  uint8_t updateSpeed;

  // -------- Inits --------- //
  initTimer0();
  OCR0B = 0;

  ANTENNA_DDR |= (1 << ANTENNA);    /* now hooked up to MOSFET, output */
  LED_DDR |= (1 << LED0);
  LED_DDR |= (1 << LED1);

  initUSART();
	printString("DC Motor Workout\r\n");

  // ------ Event loop ------ //
  while (1) {

    updateSpeed = getNumber();

                                      /* Ramp up/down to desired speed */
    if (OCR0B < updateSpeed) {
      LED_PORT |= (1 << LED0);
      while (OCR0B < updateSpeed) {
        OCR0B++;
        _delay_ms(SPEED_STEP_DELAY);
      }
    }
    else {
      LED_PORT |= (1 << LED1);
      while (OCR0B > updateSpeed) {
        OCR0B--;
        _delay_ms(SPEED_STEP_DELAY);
      }
    }
    LED_PORT = 0;                                           /* all off */

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
