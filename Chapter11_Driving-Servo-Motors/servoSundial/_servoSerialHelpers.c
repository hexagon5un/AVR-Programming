              /* Functions for serial port output formatting and input */

void printTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  printByte(hours);
  transmitByte(':');
  printByte(minutes);
  transmitByte(':');
  printByte(seconds);
  transmitByte('\r');
  transmitByte('\n');
}

void pollSerial(void) {
                          /* Poll for serial input -- to set the time. */
  char input;
  if (bit_is_set(UCSR0A, RXC0)) {
    input = UDR0;
    if (input == 'S') {                         /* enter set-time mode */
      printString("Setting time...\r\n");
      printString("Hour: ");
      hours = getNumber();
      printString("\r\nMinutes: ");
      minutes = getNumber();
      printString("\r\nSeconds: ");
      seconds = getNumber();
      printString("\r\n");
      ticks = 0;
      if ((hours >= START_TIME) && (hours < STOP_TIME)) {
        setServoPosition();
      }
    }
  }
}
