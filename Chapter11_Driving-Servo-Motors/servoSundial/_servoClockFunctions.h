

#include "servoSundial.h"
// Note: Global variables (ticks, seconds, minutes, hours)
// are declared in servoSundial.h 
// and defined in servoSundial.c

// Realtime-clock handling functions

// This sets up the interrupt clock
void initTimer0_Clock(void); 

// These functions are called periodically
// to update the global time variables
// They cascade when needed (second -> minute)
void everySecond(void); 
void everyMinute(void); 
void everyHour(void); 
