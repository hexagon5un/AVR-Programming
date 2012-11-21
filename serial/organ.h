
#define SPEAKER       PD6
#define SPEAKER_DDR   DDRD
#define SPEAKER_PORT  PORTD

#define BUTTON        PD4
#define BUTTON_DDR    DDRD
#define BUTTON_PORT   PORTD
#define BUTTON_IN     PIND

#define NOTE_DURATION     0x6000 /* determines base note length */

#define BV(bit)               (1 << bit)
#define set_bit(sfr, bit)     (_SFR_BYTE(sfr) |= BV(bit))
#define clear_bit(sfr, bit)   (_SFR_BYTE(sfr) &= ~BV(bit))
#define toggle_bit(sfr, bit)  (_SFR_BYTE(sfr) ^= BV(bit))

void playNote(uint16_t wavelength, uint16_t duration);
void rest(uint16_t duration);

