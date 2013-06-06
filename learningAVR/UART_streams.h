/* Functions to initialize stream, stdio over USART */

// To use printf, scanf, gets, puts, etc in your code, 
// #include "UART_streams.h" and
// cut and paste the following into your main()'s initialization section
/* 

initUART();
stdin = &uart_stream;
stdout = &uart_stream; 

*/

#ifndef BAUD
#define BAUD  9600        /* good safe default baud rate */
#endif

/* <util/setbaud.h> requires BAUD, F_CPU to be defined 
    in order to calculate the bit-rate multiplier.   */
#include <util/setbaud.h>	

// Inits UART, sets baud rate, etc
void initUART (void);

// Sets up the stream I/O file pointer
static int uart_putchar(char c, FILE *stream);
static int uart_getchar(FILE *stream);
FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
