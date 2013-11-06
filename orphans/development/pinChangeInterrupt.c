#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"

#include "USART.h"

ISR(PCINT1_vect) {
  LED_PORT ^= (1 << LED0);
}

static inline void initPinChangeInterrupt(void) {
  PCICR |= (1 << PCIE1);    /* enable Pin-change interrupts 1 (bank C) */
                          /* enable specific interrupt for our pin PC1 */
  PCMSK1 |= (1 << PCINT9);
  sei();                          /* set (global) interrupt enable bit */
}


int main(void) {
  LED_DDR = 0xff;
  initPinChangeInterrupt();
  CAP_SENSOR_DDR |= (1 << CAP_SENSOR);                       /* output */
  // ------ Event loop ------ //
  while (1) {

    _delay_ms(100);
    CAP_SENSOR_PORT ^= (1 << CAP_SENSOR);

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
