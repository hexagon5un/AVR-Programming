
// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "macros.h"
#include "fullSaw15.h"

#define BASEPITCH       220  /* in tuningWord steps, which are ~1/2 Hz */
#define PHASE_RATE      7          /* controls speed of phasing effect */

#define NUMBER_OSCILLATORS  16
/* 2 and 4 work just fine.
   8 and 16 take too long to maintain our 31.25kHz sample rate
   so the pitch shifts downwards and there's all sorts of aliasing.
   If you're just after scary sounds, 8 and 16 are awesome, but you may want
