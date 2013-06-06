#include <avr/io.h>             
#include <stdio.h>
#include "USART.h"
#include <avr/pgmspace.h>

char foo[] PROGMEM ="Hi, a string in progmem.";


static int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}
static int uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}
FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE  uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

int main(void) {
  initUART();
  stdout = &uart_output;
  stdin  = &uart_input;
  
  float pi = 3.141592654;    
  int16_t i = 1234; 	
  uint8_t input;
  uint8_t* inputP = &input;
  uint8_t myvalue = 64;

  while(1){
    puts_P(PSTR("Hello world!"));
    printf_P(PSTR("Type a 0-255 number: "));
    scanf_P(PSTR("%d"), &input);
    printf_P(PSTR("You typed \"%d\"\n"), input);
    
    printf_P(PSTR("%S\n"), foo);
    printf_P(PSTR("My Address: %p\n\n"), foo);
    
    printf_P(PSTR("My Integer: %#x\n"), i);
    printf_P(PSTR("My Integer: %X\n"), i);
    printf_P(PSTR("My Integer: %8d\n"), i);

    // More showing off.
    printf("Here is myvalue as an unsigned integer: %d\n", myvalue);
    printf("Here is myvalue as a char: %c\n", myvalue);
    printf("Here is myvalue in hex: 0x%X\n", myvalue);
    printf("Here is myvalue in octal: %o\n", myvalue);

    
    printf_P(PSTR("I like %.4f \n"), pi);

    
  }
  return 0;
}

