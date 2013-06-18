
/* 
  Quick and dirty functions that make serial communications work.
*/

#include <avr/io.h>		
#include "USART.h"
#include <util/setbaud.h>

void initUSART (void) {			 /* requires BAUD */
  UBRR0H = UBRRH_VALUE;			 /* defined in setbaud.h */
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif
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


/* Here are a bunch of useful printing commands */
void printString(const char myString[]){
  uint8_t i=0;
  while(myString[i]){
    transmitByte(myString[i]);
    i++;
  }
}

void printByte(uint8_t byte){
  /* Converts a byte to a string of decimal text, sends it */
  transmitByte('0'+ (byte/100));       /* Hundreds */
  transmitByte('0'+ ((byte/10) % 10)); /* Tens     */
  transmitByte('0'+ (byte % 10));      /* Ones     */
 }					

void printBinaryByte(uint8_t byte){
  /* Prints out a byte as a series of 1's and 0's */
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
