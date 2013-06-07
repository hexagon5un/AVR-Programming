/* */

// ------- Preamble -------- //
#include <avr/io.h>             
#include <avr/eeprom.h>

#include "pinDefines.h"
#include "macros.h"
#include "UART.h"

#define MAX_TEXT_LENGTH  128

// -------- Global Variables --------- //    
#define CODE_LEN   64	
char  EEMEM code0[CODE_LEN]="ettubrute";
char  EEMEM code1[CODE_LEN]="attackatdawn";
char  EEMEM code2[CODE_LEN]="theraininspainfallsmainlyontheplain";
char  EEMEM code3[CODE_LEN]="ablewasiereisawelba";
char* codePointers[] = {code0, code1, code2, code3};

char  EEMEM welcomeString[]="\r\n--==  Caesar Cipher  ==--\r\n";
char  EEMEM menuEncode[]=" [e] to encode text\r\n";
char  EEMEM menuDecode[]=" [d] to decode text\r\n\r\n";
char  EEMEM menuEnterText[]=" [n] to enter new text\r\n";
char  EEMEM menuCodeText[]=" [c] to select your code phrase\r\n";

//char  EEMEM promptCode[]=    "The code phrase is: ";
//char  EEMEM promptText[]="\r\nYour text is      : ";
char  EEMEM promptCode[]=    "code: ";
char  EEMEM promptText[]="\r\ntext: ";

char  EEMEM promptSelectCode[]="Please select your codephrase:\r\n\r\n";
char  EEMEM promptTypeText[]="Please type your text: ";

// -------- Functions --------- //

void encodeCaesar(char text[], char code[]){
  uint8_t codePosition = 0;
  uint8_t textPosition = 0;
  char thisLetter; 
  do{
    thisLetter = text[textPosition];
    if (!(thisLetter == ' ')){
      if (code[codePosition] == 0){
	codePosition = 0;
      }
      thisLetter += (code[codePosition] - 'a');
      if (thisLetter > 'z'){
	thisLetter -= 26;
      }
    }
    text[textPosition] = thisLetter;
    codePosition++;
    textPosition++;
  } while(text[textPosition]);
}

void decodeCaesar(char text[], char code[]){
  uint8_t codePosition = 0;
  uint8_t textPosition = 0;
  char thisLetter;
  do{
    thisLetter = text[textPosition];
    if (!(thisLetter == ' ')){
      if (code[codePosition] == 0){
	codePosition = 0;
      }
      thisLetter -= (code[codePosition] - 'a');
      if (thisLetter < 'a'){
	thisLetter += 26;
      }
    }
    text[textPosition] = thisLetter;
    codePosition++;
    textPosition++;
  } while(text[textPosition]);
}

void enterText(char text[]){
  uint8_t i;
  char letter;
  printFromEEPROM(promptTypeText);
  do{
    letter = receiveByte();
    transmitByte(letter);	/* echo */
    text[i] = letter;
    i++;
  } while(!(letter == '\r') && (i < (MAX_TEXT_LENGTH-1)));
  text[i-1] = 0;
}

void selectCode(char code[]){
  uint8_t i;
  char input;
  char* codeAddress;
  eeprom_read_block(code, promptSelectCode, CODE_LEN);
  printString(code);
  do{
    for (i=0; i<4; i++){
      codeAddress = codePointers[i];
      eeprom_read_block(code, codeAddress, CODE_LEN);
      transmitByte(' ');
      transmitByte('0'+i);
      printString(": ");
      printString(code);
      printString("\r\n");
    }
    input = receiveByte();
  } while((input < '0') || (input > '3'));
  codeAddress = codePointers[input-'0'];
  eeprom_read_block(code, codeAddress, CODE_LEN);
}

void printFromEEPROM(char* eepromString){
  char buffer[MAX_TEXT_LENGTH];
  eeprom_read_block(buffer, eepromString, MAX_TEXT_LENGTH);
  printString(buffer);
}


int main(void){
  // -------- Inits --------- //
  
  char textBuffer[MAX_TEXT_LENGTH];	
  char codeString[CODE_LEN];
  
  char input;
  uint8_t i;
  initUART();

  
  // ------ Event loop ------ //
  while(1){     
    
    // Menu
    printFromEEPROM(welcomeString);
    
    printFromEEPROM(promptText);
    printString(textBuffer);
    printString("\r\n");
    printFromEEPROM(promptCode);
    printString(codeString);
    printString("\r\n\r\n ---------------------\r\n");
    printFromEEPROM(menuEnterText);
    printFromEEPROM(menuCodeText);
    printFromEEPROM(menuEncode);
    printFromEEPROM(menuDecode);

    input = receiveByte();

    switch(input){
    case 'e':
      encodeCaesar(textBuffer, codeString);
      break;
    case 'd':
      decodeCaesar(textBuffer, codeString);
      break;
    case 'n':
      enterText(textBuffer);
      break;
    case 'c':
      selectCode(codeString);
      break;
    }
    
    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

