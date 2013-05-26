/* Functions to initialize, send, receive over USART
   
   initUSART requires BAUD to be defined in order to calculate 
     the bit-rate multiplier.  9600 is a reasonable default.
   At 8MHz, you can try 19200, 38400 baud.  But you might like to 
     test them out on long strings of text before using.
 */

#ifndef BAUD
#define BAUD  9600
// #define BAUD 19200
// #define BAUD 38400
#endif
#include <util/setbaud.h>

/* Takes the defined BAUD and F_CPU, 
   calculates the bit-clock multiplier,
   and configures the hardware USART                   */ 
void initUSART(void);

/* Blocking transmit and receive functions.
   When you call receiveByte() your program will hang until
   data comes through.  We'll improve on this later. */
void transmitByte(uint8_t data);
uint8_t receiveByte(void);

void printString(char *string);
/* Utility function to transmit an entire string from RAM */

void printByte(uint8_t byte);
/* Prints a byte out as its 3-digit ascii equivalent */
void printWord(uint16_t word);
/* Prints a word (16-bits) out as its 5-digit ascii equivalent */

void printBinaryByte(uint8_t byte);
/* Prints a byte out in 1s and 0s */
char nibbleToHex(uint8_t nibble);
void printHexByte(uint8_t byte);
/* Prints a byte out in hexadecimal */
uint8_t getNumber(void);
/* takes in up to three ascii digits, 
 converts them to a byte when press enter */

