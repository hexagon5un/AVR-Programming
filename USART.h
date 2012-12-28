
/* 
   Functions to initialize, send, receive over USART
   
   initUSART requires BAUDRATE to be defined in order to calculate 
     the bit-rate multiplier.  9600 is a reasonable default.

 */


#ifndef BAUDRATE
#define BAUDRATE  9600
#endif

/* Takes the defined BAUDRATE, calculates the bit-clock multiplier,
   and configures the hardware USART                   */ 
void initUSART(void);

/* Blocking transmit and receive functions.
   When you call receiveByte() your program will hang until
   data comes through.  We'll improve on this later.
*/
void transmitByte(uint8_t data);
uint8_t receiveByte(void);

/* Utility functions to transmit entire strings at a time. */
void transmitString(char *string);
void printByte(uint8_t byte);
void sayOK(void);



