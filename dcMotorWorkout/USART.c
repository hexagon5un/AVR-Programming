
/* 
  Quick and dirty functions that make serial communications work.

  Note that receiveByte() blocks -- it sits and waits _forever_ for
   a byte to come in.  If you're doing anything that's more interesting,
   you'll want to implement this with interrupts.

   initUSART requires BAUDRATE to be defined in order to calculate 
     the bit-rate multiplier.  9600 is a reasonable default.

  May not work with some of the older chips: 
    Tiny2313, Mega8, Mega16, Mega32 for instance have different pin macros
    If you're using these chips, see (e.g.) iom8.h for how it's done right.
    These old chips predate 2 USARTs per chip, and so don't specify UDR0 vs UDR1.
    Correspondingly, the macros will just be defined as UDR.
    You can drop zeros from all of the macro names here, and it might work.
*/

#include <avr/io.h>		
#include "UART.h"

void initUSART (void) {			 /* requires BAUDRATE */
  if ((F_CPU / 16 / BAUDRATE - 1) < 20){ /* switch to double-speed if too low */
    UCSR0A |= (1 << U2X0);                
    UBRR0L = F_CPU / 8 / BAUDRATE - 1;
  }
  else{
    UBRR0L = F_CPU / 16 / BAUDRATE - 1; 
  }
  UCSR0B = (1 << TXEN0) | (1 << RXEN0); /* Enable USART transmitter/receiver */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8 data bits, 1 stop bit */
}

void transmitByte (uint8_t data) {
  loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait for empty transmit buffer */
  UDR0 = data;			          /* send data */  
}

uint8_t receiveByte (void) {
  loop_until_bit_is_set(UCSR0A, RXC0); /* Wait for incoming data */
  return UDR0;			         /* return register value */  
}



/* Here follow a bunch of useful printing commands */

void printString(char *string){
  uint8_t i=0;
  while(string[i]){
    transmitByte(string[i]);
    i++;
  }
}

void printByte(uint8_t byte){
  /* Converts a byte to a string of decimal text, sends it */
  uint8_t tens;
  tens = byte / 10;
  transmitByte( ((tens/10) % 10) + '0');
  transmitByte( (tens % 10) + '0');
  transmitByte( (byte % 10) + '0');
}

void printBinaryByte(uint8_t byte){
  uint8_t bit = 8;
  while(bit){
    bit--;
    if ( (1<<bit) & byte ){
      transmitByte('1');
    }
    else{
      transmitByte('0');
    }
  }
}

char nibbleToHexCharacter(uint8_t nibble){
  if (nibble < 10){
    return('0' + nibble);
  }
  else{
    return('A' + nibble - 10);
  }
}

void printHexByte(uint8_t byte){
  uint8_t nibble;
  nibble = (byte & 0b11110000) >> 4;
  transmitByte(nibbleToHexCharacter(nibble));
  nibble = byte & 0b00001111;
  transmitByte(nibbleToHexCharacter(nibble));
}
