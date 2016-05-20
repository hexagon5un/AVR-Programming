/*
   Capacitive touch sensor demo
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include "pinDefines.h"
#include "USART.h"

#define SENSE_TIME   50
#define THRESHOLD    12000

// -------  Global Variables ---------- //
volatile uint16_t chargeCycleCount;

// ------- Functions -------- //

void initPinChangeInterrupt(void) {
  PCICR |= (1 << PCIE1);    /* enable Pin-change interrupts 1 (bank C) */
  PCMSK1 |= (1 << PC1);   /* enable specific interrupt for our pin PC1 */
}

ISR(PCINT1_vect) {
  chargeCycleCount++;                             /* count this change */

  CAP_SENSOR_DDR |= (1 << CAP_SENSOR);                  /* output mode */
  _delay_us(1);                                      /* charging delay */

  CAP_SENSOR_DDR &= ~(1 << CAP_SENSOR);                /* set as input */
  PCIFR |= (1 << PCIF1);             /* clear the pin-change interrupt */
}


int main(void) {
  // -------- Inits --------- //
  clock_prescale_set(clock_div_1);                       /* full speed */
  initUSART();
  printString("==[ Cap Sensor ]==\r\n\r\n");

  LED_DDR = 0xff;
  MCUCR |= (1 << PUD);                          /* disable all pullups */
  CAP_SENSOR_PORT |= (1 << CAP_SENSOR);    /* we can leave output high */

  initPinChangeInterrupt();

  // ------ Event loop ------ //
  while (1) {

    chargeCycleCount = 0;                             /* reset counter */
    CAP_SENSOR_DDR |= (1 << CAP_SENSOR);     /* start with cap charged */
    sei();                            /* start up interrupts, counting */
    _delay_ms(SENSE_TIME);
    cli();                                                     /* done */
    if (chargeCycleCount < THRESHOLD) {
      LED_PORT = 0xff;
    }
    else {
      LED_PORT = 0;
    }
    printWord(chargeCycleCount);                    /* for fine tuning */
    printString("\r\n");

  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}
