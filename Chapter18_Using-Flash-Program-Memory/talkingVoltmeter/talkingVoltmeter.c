//  Talking Voltmeter Example

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include "pinDefines.h"
#include "USART.h"

#include "talkingVoltmeter.h"

void startSampleTimer(void) {
  sampleNumber = 0;                   /* back to start of sample table */
  TCCR2B = (1 << CS21);                         /* turn on timer clock */
                /* Two clock options above end up ~8kHz on 8MHz system */
}
void stopSampleTimer(void) {
  TCCR2B = 0;                         /* disable sample-playback clock */
  OCR0A = 128;                              /* idle PWM at mid-voltage */
  lastout = 0;                                 /* start at 0 next time */
}
void speak(void) {
  startSampleTimer();
  loop_until_bit_is_clear(TCCR2B, CS21);            /* Wait until done */
}

void updatePWMAudio(void) {
  OCR0A = out + 128;                        /* re-center for 0-255 PWM */
  lastout = out;                                  /* update last value */
  sampleNumber++;                                 /* on to next sample */
}
void unpackByte(uint8_t dataByte) {
                           /* Select pairs of bits from byte, save out */
  differentials[0] = (dataByte >> 6) & 0b00000011;
  differentials[1] = (dataByte >> 4) & 0b00000011;
  differentials[2] = (dataByte >> 2) & 0b00000011;
  differentials[3] = (dataByte & 0b00000011);
}

/* Timer 2 controls sampling speed.
   ISR reads new data, loads PWM values into OCR0A */
ISR(TIMER2_COMPA_vect) {
  /* Since we can decode 4 2-bit values at once, need to know where
     we are in the 4-step mini-cycle. */
  uint8_t cycle = sampleNumber & 0b00000011;       /* keep last 2 bits */
  uint16_t tableEntry;
  uint8_t packedData;

  if (cycle == 0) {                        /* at first sample, re-load */
    tableEntry = sampleNumber >> 2;           /* where we are in table */
    if (tableEntry < thisTableLength) {
                         /* read the next byte from the selected table */
      packedData = pgm_read_byte(&thisTableP[tableEntry]);
      unpackByte(packedData);    /* split up byte into differentials[] */
    }
    else {                                   /* at end of table, done. */
      stopSampleTimer();
    }
  }
          /* Decode the differences: current value = last + difference */
  out = lastout + dpcmWeights[differentials[cycle]] - (lastout >> 4);
  updatePWMAudio();
}                               // end  ISR (TIMER2_COMPA_vect)

void printString_Progmem(const char *stringP) {
  char oneLetter;
  while ((oneLetter = pgm_read_byte(stringP))) {
    transmitByte(oneLetter);
    stringP++;
  }
}


int main(void) {
  uint16_t voltage;
  uint8_t volts;
  uint8_t tenths;
  uint8_t vcc = 51;                               /* 10x VCC, in volts */

  clock_prescale_set(clock_div_1);                            /* 8 MHz */
  initTimer0();
  initTimer2();
  sei();                                             /* for timer2 ISR */
  initADC();
  initUSART();

  printString_Progmem(PSTR("\r\n--=( Talking Voltmeter )=--\r\n"));

  selectTable(INTRO);
  speak();

  while (1) {

    ADCSRA |= (1 << ADSC);                                /* start ADC */
    loop_until_bit_is_clear(ADCSRA, ADSC);

    voltage = ADC * vcc + vcc / 2;      /* vcc/2 to make rounding work */
    voltage = voltage >> 10;              /* divide by 10-bits for ADC */
                   /* "voltage" is now actually 10x real-world voltage */
    volts = voltage / 10;
    tenths = voltage % 10;

    transmitByte('0' + volts);                /* serial output as well */
    selectTable(volts);                 /* 0 points to ZERO_TABLE, etc */
    speak();

    transmitByte('.');
    selectTable(POINT);
    speak();

    transmitByte('0' + tenths);
    selectTable(tenths);
    speak();

    printString_Progmem(PSTR("  volts\r\n"));
    selectTable(VOLTS);
    speak();

    _delay_ms(SPEECH_DELAY);

  }                                                       /* end while */
  return 0;
}
