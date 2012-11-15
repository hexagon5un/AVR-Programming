
/* 
  Simple functions that make serial comms easy.
  
  Requires BAUDRATE to be defined for initUART

  May not work with some of the older chips: 

*/

void transmitByte (uint8_t data) {
  while (!(UCSR0A & (1 << UDRE0))); /* Wait for empty transmit buffer */
  UDR0 = data;			/* send data */  
}

uint8_t receiveByte (void) {
  while (!(UCSR0A & (1 << RXC0))); /* Wait for incoming data */
  return UDR0;			/* return register value */  
}

void initUART (void) {			 /* requires BAUDRATE */
  if ((F_CPU / 16 / BAUDRATE - 1) < 20){ /* switch to double-speed if too low */
    UCSR0A |= (1 << U2X0);                
    UBRR0L = F_CPU / 8 / BAUDRATE - 1;
  }
  else{
    UBRR0L = F_CPU / 16 / BAUDRATE - 1; 
  }

  UCSR0B = (1 << TXEN0) | (1 << RXEN0); /* Enable UART transmitter/receiver */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8 data bits, 1 stop bit */
}


void sayOK(void){
  transmitByte('O');
  transmitByte('K');
  transmitByte('\n');
}
