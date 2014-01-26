/*
   Direct-digital synthesis
   ADSR Dynamic Volume Envelope Demo
*/

// ------- Preamble -------- //
#include "adsr.h"             /* Defines, includes, and init functions */

int main(void) {

  // -------- Inits --------- //

  uint16_t accumulator = 0;
  uint8_t volume = 0;
  uint16_t noteClock = 0;
  uint16_t tuningWord = C1;

  uint8_t waveStep;
  int16_t mixer;
  uint8_t i;
  char serialInput;

  clock_prescale_set(clock_div_1);                  /* CPU clock 8 MHz */
  initTimer0();
  initUSART();
  printString("  Serial Synth\r\n");
  printString("Notes: asdfghjkl;'\r\n");

  SPEAKER_DDR |= (1 << SPEAKER);                     /* speaker output */

  // ------ Event loop ------ //
  while (1) {

    // Set PWM output
    loop_until_bit_is_set(TIFR0, TOV0);    /* wait for timer0 overflow */
    OCR0A = 128 + (uint8_t) mixer;
    TIFR0 |= (1 << TOV0);                    /* reset the overflow bit */

    // Update the DDS
    accumulator += tuningWord;
    waveStep = accumulator >> 8;
    mixer = fullTriangle[waveStep] * volume;
    mixer = mixer >> 5;

                                  /* Input processed here: check USART */
    if (bit_is_set(UCSR0A, RXC0)) {
      serialInput = UDR0;                        /* read in from USART */
      tuningWord = lookupPitch(serialInput);
      noteClock = 1;
    }

                                          /* Dynamic Volume stuff here */
    if (noteClock) {                   /* if noteClock already running */
      noteClock++;
      if (noteClock < ATTACK_TIME) {                         /* attack */
                              /* wait until time to increase next step */
        if (noteClock > ATTACK_RATE * volume) {
          if (volume < 31) {
            volume++;
          }
        }
      }
      else if (noteClock < DECAY_TIME) {                      /* decay */
        if ((noteClock - ATTACK_TIME) >
            (FULL_VOLUME - volume) * DECAY_RATE) {
          if (volume > SUSTAIN_LEVEL) {
            volume--;
          }
        }
      }
      else if (noteClock > RELEASE_TIME) {                  /* release */
        if ((noteClock - RELEASE_TIME) >
            (SUSTAIN_LEVEL - volume) * RELEASE_RATE) {
          if (volume > 0) {
            volume--;
          }
          else {
            noteClock = 0;
          }
        }
      }
    }


  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
