                                                           /* Includes */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"

#include "USART.h"

                                                   /* Global Variables */
volatile uint16_t chargeCycleCount;

ISR(TIMER0_COMPA_vect) {
                                       /* run when timer compare match */
  transmitByte((uint8_t) (chargeCycleCount >> 4));

  if (chargeCycleCount == 0) {       /* trigger charge/discharge cycle */
    CAP_SENSOR_DDR |= (1 << CAP_SENSOR);
    CAP_SENSOR_PORT |= (1 << CAP_SENSOR);
  }

  chargeCycleCount = 0;
  LED_PORT ^= (1 << LED1);
}

static inline void initTimer0(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0B |= (1 << CS02);                            /* 8 MHz / 8 clock */
  TCCR0B |= (1 << CS00);                            /* 8 MHz / 8 clock */
  TIMSK0 |= (1 << OCIE0A);          /* output compare interrupt enable */
  OCR0A = 200;                                    /* 8 Mhz / 8 /  36 = */
  sei();                                   /* set enable interrupt bit */
}

ISR(PCINT1_vect) {
  LED_PORT |= (1 << LED0);
  chargeCycleCount++;                               /* count new value */

                                     /* set low to discharge capacitor */
  CAP_SENSOR_PORT &= ~(1 << CAP_SENSOR);
  CAP_SENSOR_DDR |= (1 << CAP_SENSOR);                  /* output mode */
  _delay_us(1);                                       /* a short while */

  CAP_SENSOR_DDR &= ~(1 << CAP_SENSOR);               /* set as input */
                                     /* set pullup to charge capacitor */
  CAP_SENSOR_PORT |= (1 << CAP_SENSOR);
  PCIFR |= (1 << PCIF1);
  LED_PORT &= ~(1 << LED0);
  cli();
}

static inline void initPinChangeInterrupt(void) {
                                            /* B, C, D = IE0, IE1, IE2 */
  PCICR |= (1 << PCIE1);    /* enable Pin-change interrupts 1 (bank C) */
                          /* enable specific interrupt for our pin PC1 */
  PCMSK1 |= (1 << PCINT9);
  sei();                          /* set (global) interrupt enable bit */
}


int main(void) {


  // -------- Inits --------- //
  initUSART();
  printString("Better Capacitive Sensor\r\n");
  LED_DDR = 0xff;
  initTimer0();
  initPinChangeInterrupt();

  // ------ Event loop ------ //
  while (1) {

    ;

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
