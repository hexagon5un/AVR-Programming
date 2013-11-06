// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "pinDefines.h"
#include "macros.h"
#include "USART.h"

volatile uint8_t currentCharacter = 0;
volatile char serialBuffer[80];

ISR(USART_UDRE_vect) {         /* Data-register empty interrupt vector */
                                    /* tests if have character to send */
  if (serialBuffer[currentCharacter]) {
    UDR0 = serialBuffer[currentCharacter];
    currentCharacter++;                   /* advance to next character */
  }
  else {                                        /* sent last character */
    clear_bit(UCSR0B, UDRIE0);
    currentCharacter = 0;
  }
}

int main(void) {
  // Event Loop
  uint8_t i;

  initUSART();
  sei();
  char testString[] = "Hello There!\r\n";

  while (1) {

    strcpy(serialBuffer, testString);       /* copy into output buffer */
    set_bit(UCSR0B, UDRIE0);           /* enable USART empty interrupt */
    _delay_ms(1000);                                /* wait a while... */

  }
}
