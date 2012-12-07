
// This is the POV blink delay time 
// -- how long each LED strip is lit.
// Anything from 500us to 2ms is good.  
// Depends on how fast you swing your arms.
#define DELAYTIME      2	/* POV, milliseconds */

// Allows you to write out strings by POV
// Routine looks up characters in the font table,
//  displays them one by one.
void POVChar(char character);
void POVString(char *myString);

// Turns off lights and pauses for 10x DELAYTIME
void pause(void);

// This routine takes any length data (invaders, 1up below)
//  and displays it a la POV.
// You must provide the length of the data
// A cute way to do this is with sizeof(data)/sizeof(uint8_t)
//   in the function call
void POVDisplay(uint8_t *povData, uint8_t numberRows);

// Does cylon eyes, but really fast.  Makes a nice zipper-like POV effect.
void cylonPOV(void);
