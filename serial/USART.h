
/* 
   Functions to initialize, send, receive over USART
   
   initUSART requires BAUDRATE to be defined in order to calculate 
     the bit-rate multiplier.  9600 is a reasonable default.

 */


#ifndef BAUDRATE
#define BAUDRATE  9600
#endif

void initUSART(void);

void transmitByte(uint8_t data);
uint8_t receiveByte(void);

void transmitString(char *string);

void sayOK(void);


