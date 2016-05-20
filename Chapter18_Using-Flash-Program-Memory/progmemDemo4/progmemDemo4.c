                              /* Fourth step into using program memory */
                           /* Passing data array pointers to functions */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "USART.h"

const uint16_t myData[] PROGMEM =
    { 1111, 2222, 3333, 4444, 5555, 6666, 7777, 8888, 9999, 10000 };
const uint16_t myData2[] PROGMEM = { 123, 456, 789, 012, 345, 678, 999 };

void printData_Progmem(const uint16_t * dataPointer, uint8_t length) {
  while (length) {
    printWord((uint16_t) dataPointer);            /* print out address */
    printString(":  ");
    printWord(pgm_read_word(dataPointer));           /* print out data */
    printString("\r\n");
    dataPointer++;                                /* move to next byte */
    length--;                                   /* one less byte to go */
    _delay_ms(100);
  }
}

int main(void) {
  initUSART();
  while (1) {
    printData_Progmem(myData, sizeof(myData) / 2);
    printString("\r\n");
    _delay_ms(1000);
    printData_Progmem(myData2, sizeof(myData2) / sizeof(myData2[0]));
    printString("\r\n");
    _delay_ms(1000);
  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}
