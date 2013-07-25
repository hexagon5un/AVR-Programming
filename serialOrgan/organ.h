
// --------------- Program Parameters --------------//
#define NOTE_DURATION     0xA000 /* determines longest note length */
                                 /* note that 2x will overflow */
                                 /* change this to tweak the default tempo */
#define TIMEBASE        1	 /* microseconds, determines base pitch range */

// ------------- Function prototypes -------------- //

// Plays a note for the given duration.  None of these times are 
//  calibrated to actual notes or tempi.  It's all relative to TIMEBASE.
void playNote(uint16_t wavelength, uint16_t duration);

// Does nothing for a time equal to the passed duration.
void rest(uint16_t duration);

