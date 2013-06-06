/* Streams demo */

#include <avr/io.h>             
#include <stdio.h>
#include <avr/pgmspace.h>
#include "USART.h"

static int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}
static int uart_getchar(FILE *stream) {
  char thisCharacter;
  loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
  thisCharacter = UDR0;		       /* store data */
  uart_putchar(thisCharacter, stream); /* echo */
  return thisCharacter;		      
}
FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

int main(void) {
  initUSART();
  stdin = &uart_stream;
  stdout = &uart_stream;
 
  uint16_t input;
  uint8_t numberConversions;

  puts_P(PSTR("\r\n------    STDIO DEMO!    ------\r\n"));

  while(1){
    printf_P(PSTR("\nType a number: "));
    numberConversions = scanf_P(PSTR("%d"), &input);
    if (numberConversions == 0){ /* failed numeric conversion routine */
      puts_P(PSTR("?   You call that a number?!"));
      puts_P(PSTR("Interpreting as a character..."));
      input = getchar();	/* read in as character to clear buffer */
    }
    
    // Show off.
    puts("");
    if (input < 128){		/* in ASCII range */
      printf("\t ASCII char:          \"%c\"\n", input);
    }
    
    printf("\t uint8_t:             %u\n", (uint8_t) input);
    printf("\t signed integer:      %d\n", input);
    printf("\t unsigned integer:    %u\n", input);
    printf("\t in hex:              %#x\n", input);
    printf("\t in octal:            %#o\n", input);
    printf("\t memory address:      %p\n", &input);
  }
  return 0;
}

