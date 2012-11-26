
// --------------- Program Parameters --------------//
#define NOTE_DURATION     0xA000 /* determines longest note length */
                                 /* note that 2x will overflow */
                                 /* change this to tweak the default tempo */

// ------------ Bit twiddle utilities ------------- //
#define BV(bit)               (1 << bit)
#define set_bit(sfr, bit)     (_SFR_BYTE(sfr) |= BV(bit))
#define clear_bit(sfr, bit)   (_SFR_BYTE(sfr) &= ~BV(bit))
#define toggle_bit(sfr, bit)  (_SFR_BYTE(sfr) ^= BV(bit))

// ------------- Function prototypes -------------- //

// Plays a note for the given duration.  None of these times are 
//  calibrated to actual notes or tempi.  It's all relative.
void playNote(uint16_t wavelength, uint16_t duration);

// Does nothing for a time equal to the passed duration.
void rest(uint16_t duration);

