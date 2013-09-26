
#include <avr/io.h>
#include "USART.h"

void initUSART(void) {                            /* requires BAUDRATE */
                                  /* switch to double-speed if too low */
  if ((F_CPU / 16 / BAUDRATE - 1) < 20) {
    UCSR0A |= (1 << U2X0);
    UBRR0L = F_CPU / 8 / BAUDRATE - 1;
  }
  else {
    UBRR0L = F_CPU / 16 / BAUDRATE - 1;
  }
                                  /* Enable USART transmitter/receiver */
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   /* 8 data bits, 1 stop bit */
}

void transmitByte(uint8_t data) {
                                     /* Wait for empty transmit buffer */
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = data;                                            /* send data */
}

uint8_t receiveByte(void) {
  loop_until_bit_is_set(UCSR0A, RXC0);       /* Wait for incoming data */
  return UDR0;                                /* return register value */
}

void printString(char *string) {
  uint8_t i = 0;
  while (string[i]) {
    transmitByte(string[i]);
    i++;
  }
}

void sayOK(void) {
  printString("OK\r\n");
}
