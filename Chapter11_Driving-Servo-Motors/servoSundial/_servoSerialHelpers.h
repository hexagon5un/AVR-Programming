              /* Functions for serial port output formatting and input */

#include "servoSundial.h"
// Prints out the time, nicely formatted
void printTime(uint8_t hours, uint8_t minutes, uint8_t seconds); 
// Polls for serial input
// sets the time if receives an "S"
void pollSerial(void); 
