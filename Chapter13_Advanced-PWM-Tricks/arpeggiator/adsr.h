
                               /* Include file for ADSR synth project. */

#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "macros.h"
#include "scale.h"
#include "fullSaw7.h"             /* a 7-harmonic bandlimited sawtooth */
                     /* see generateWavetables.py if you're interested */
#include "fullTriangle.h"
#include "USART.h"

#define FULL_VOLUME     31                            /* 5-bit volumes */

// Default envelope values (slightly percussive)
// Play around with these!
#define ATTACK_RATE    8                                      /* 0-255 */
#define DECAY_RATE     120                                    /* 0-255 */
#define SUSTAIN_LEVEL  25                                     /* 0-255 */
#define SUSTAIN_TIME   4000                                 /* 0-65535 */
#define RELEASE_RATE   200                                  /* 0-65535 */

// Compute these constants
#define ATTACK_TIME   ATTACK_RATE * FULL_VOLUME
#define DECAY_TIME   (ATTACK_TIME + (FULL_VOLUME-SUSTAIN_LEVEL) * DECAY_RATE)
