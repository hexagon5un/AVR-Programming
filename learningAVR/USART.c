
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
  transmitByte('0'+ (byte/100));	 /* Hundreds */
  transmitByte('0'+ ((byte/10) % 10)); /* Tens     */
  transmitByte('0'+ (byte % 10));	 /* Ones     */
}					

void printWord(uint16_t word){
  transmitByte('0'+ (word/10000));         /* Ten-thousands */
  transmitByte('0'+ ((word/1000) % 10));   /* Thousands    */
  transmitByte('0'+ ((word/100) % 10));    /* Hundreds */
  transmitByte('0'+ ((word/10) % 10));     /* Tens     */
  transmitByte('0'+ (word % 10));	     /* Ones     */
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

char nibbleToHexCharacter(uint8_t nibble){
  /* Converts 4 bits into hexadecimal */
  if (nibble < 10){
    return('0' + nibble);
  }
  else{
    return('A' + nibble - 10);
  }
}

void printHexByte(uint8_t byte){
  /* Prints a byte as its hexadecimal equivalent */
  uint8_t nibble;
  nibble = (byte & 0b11110000) >> 4;
  transmitByte(nibbleToHexCharacter(nibble));
  nibble = byte & 0b00001111;
  transmitByte(nibbleToHexCharacter(nibble));
}

uint8_t getNumber(void){
  // Gets a numerical 0-255 from the serial port.
  // Converts from string to number.
  char hundreds='0'; 
  char tens='0';
  char ones = '0';
  char thisChar = '0';
  do{    			/* shift over */
    hundreds = tens;            
    tens = ones;
    ones = thisChar;
    thisChar = receiveByte();   /* get a new character */
    transmitByte(thisChar);     /* echo */
  } while(thisChar != '\r');	/* until type return */
  return(100*(hundreds-'0') + 10*(tens-'0') + ones-'0');
}

