
#include <avr/io.h>
#include <avr/eeprom.h>
#include "USART.h"

#define MAX_TEXT_LENGTH  256
#define CODE_LEN         64

// -------- Global Variables --------- //
char EEMEM code0[CODE_LEN] = "ettubrute";
char EEMEM code1[CODE_LEN] = "attackatdawn";
char EEMEM code2[CODE_LEN] = "theraininspainfallsmainlyontheplain";
char EEMEM code3[CODE_LEN] = "ablewasiereisawelba";
char *codePointers[] = { code0, code1, code2, code3 };

// Menu strings.  Why not store them in EEPROM?
char EEMEM welcomeString[] = "\r\n--==  Vigenere Cipher  ==--\r\n";
char EEMEM menuEncode[] = " [e] to encode text\r\n";
char EEMEM menuDecode[] = " [d] to decode text\r\n\r\n";
char EEMEM menuEnterText[] = " [n] to enter new text\r\n";
char EEMEM menuCodeText[] = " [c] to select your code phrase\r\n";
char EEMEM menuChangeCode[] = " [x] to modify code phrases\r\n";
char EEMEM promptCode[] = "code: ";
char EEMEM promptText[] = "\r\ntext: ";

char EEMEM promptSelectCode[] = "Select codephrase:\r\n\r\n";
char EEMEM promptTypeText[] = "Type your text: ";

// Given the address of an EEPROM string, prints it out
// Used for menu items
void printFromEEPROM(char *eepromString);

// Takes input from serial, stores it in the text array
void enterText(char text[]);

// Reads code phrases out of EEPROM and prints them.
// Uses the codeBuffer for temporary storage
void displayCodes(void);

// Changes a code phrase, both in EEPROM and the current code
void changeCode(char codeBuffer[]);

// Pick a code phrase from EEPROM
void selectCode(char code[]);

// Encodes the passed text string, in place
void encodeVigenere(char text[], char code[]);

// Decodes the passed text string, in place
void decodeVigenere(char text[], char code[]);
