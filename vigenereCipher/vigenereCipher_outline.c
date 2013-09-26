/*
Vigenere Cipher encoder / decoder demo
And an excuse to play around with EEPROM memory
 */

#include "vigenereCipher.h"

// -------- Functions --------- //

void printFromEEPROM(char *eepromString);
void enterText(char text[]);
void displayCodes(void);
void changeCode(char codeBuffer[]);
void selectCode(char code[]);
void encodeVigenere(char text[], char code[]);
void decodeVigenere(char text[], char code[]);

int main(void) {
  // -------- Inits --------- //

  char textBuffer[MAX_TEXT_LENGTH];
  char codeString[CODE_LEN];
  char input;
  initUSART();

  // ------ Event loop ------ //
  while (1) {

    // Menu
    printFromEEPROM(welcomeString);
    // .... more fany menu printing
    printFromEEPROM(menuDecode);
    input = receiveByte();

    switch (input) {
    case 'e':                  // encode
      encodeVigenere(textBuffer, codeString);
      break;
    case 'd':                  // decode
      decodeVigenere(textBuffer, codeString);
      break;
    case 'n':                  // new text
      printFromEEPROM(promptTypeText);
      enterText(textBuffer);
      break;
    case 'c':                  // choose code
      selectCode(codeString);
      break;
    case 'x':                  // change code
      changeCode(codeString);
      break;
    }
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
