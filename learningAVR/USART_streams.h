/* Functions to initialize stream, stdio over USART */

// To use printf, scanf, gets, puts, etc in your code, 
// #include "USART_streams.h" and
// cut and paste the following into your main()'s initialization section
/* 

initUSART();
stdin = &uart_stream;
stdout = &uart_stream; 

*/
#include <avr/io.h>		
#include <stdio.h>		

/* <util/setbaud.h> requires BAUD, F_CPU to be defined 
    in order to calculate the bit-rate multiplier.   */
#ifndef BAUD
#define BAUD  9600        /* good safe default baud rate */
#endif
#include <util/setbaud.h>	


// Inits USART, sets baud rate, etc
void initUSART (void);

// Sets up the stream I/O file pointer
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

// Define uart_stream when called by main(), not by USART_streams.c
#ifndef USART_STREAMS_C
FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
#endif
