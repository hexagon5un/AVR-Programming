
#include <avr/io.h>             
#include <avr/eeprom.h>
#include "UART.h"

#define MAX_TEXT_LENGTH  256
#define CODE_LEN         64	

// -------- Global Variables --------- //    
char  EEMEM code0[CODE_LEN]="ettubrute";
char  EEMEM code1[CODE_LEN]="attackatdawn";
char  EEMEM code2[CODE_LEN]="theraininspainfallsmainlyontheplain";
char  EEMEM code3[CODE_LEN]="ablewasiereisawelba";
char* codePointers[] = {code0, code1, code2, code3};

// Menu strings.  Why not store them in EEPROM?  
char  EEMEM welcomeString[]="\r\n--==  Caesar Cipher  ==--\r\n";
char  EEMEM menuEncode[]=" [e] to encode text\r\n";
char  EEMEM menuDecode[]=" [d] to decode text\r\n\r\n";
char  EEMEM menuEnterText[]=" [n] to enter new text\r\n";
char  EEMEM menuCodeText[]=" [c] to select your code phrase\r\n";
char  EEMEM menuChangeCode[]=" [x] to modify code phrases\r\n";
char  EEMEM promptCode[]=    "code: ";
char  EEMEM promptText[]="\r\ntext: ";

char  EEMEM promptSelectCode[]="Select codephrase:\r\n\r\n";
char  EEMEM promptTypeText[]="Type your text: ";

void printFromEEPROM(char* eepromString);
// Given the address of an EEPROM string, prints it out
// Used for menu items

void enterText(char text[]);
// Takes input from serial, stores it in the text array

void displayCodes(char codeBuffer[]);
// Reads code phrases out of EEPROM and prints them.
// Uses the codeBuffer for temporary storage

void changeCode(char codeBuffer[]);
// Changes a code phrase, both in EEPROM and the current code

void selectCode(char code[]);
// Pick a code phrase from EEPROM

void encodeCaesar(char text[], char code[]);
// Encodes the passed text string, in place

void decodeCaesar(char text[], char code[]);
// Decodes the passed text string, in place

