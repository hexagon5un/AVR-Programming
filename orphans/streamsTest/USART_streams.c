
#define USART_STREAMS_C
#include "USART_streams.h"

// Enables USART hardware, sets baud rate
void initUSART(void) {                                /* requires BAUD */
  UBRR0H = UBRRH_VALUE;                        /* defined in setbaud.h */
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif
                                  /* Enable USART transmitter/receiver */
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   /* 8 data bits, 1 stop bit */
}

// Rudimentary send.  Converts \n to \r\n.
int uart_putchar(char c, FILE * stream) {
  if (c == '\n') {
    uart_putchar('\r', stream);
  }
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
  return 0;
}

// Echoing receive
int uart_getchar(FILE * stream) {
  char thisCharacter;
  loop_until_bit_is_set(UCSR0A, RXC0);      /* Wait until data exists. */
  thisCharacter = UDR0;                                  /* store data */
  uart_putchar(thisCharacter, stream);                         /* echo */
  return thisCharacter;
}
