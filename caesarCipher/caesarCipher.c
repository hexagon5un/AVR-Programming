/*
Caesar Cipher encoder / decoder demo
And an excuse to play around with EEPROM memory
 */

#include "caesarCipher.h"

// -------- Functions --------- //

void printFromEEPROM(char* eepromString){
  uint8_t letter;
  do {
    letter = eeprom_read_byte((uint8_t*) eepromString);
    transmitByte(letter);
    eepromString++;
  } while(letter);
}

void enterText(char text[]){
  uint8_t i=0;
  char letter;
  do{
    letter = receiveByte();
    transmitByte(letter);	/* echo */
    text[i] = letter;
    i++;
  } while(!(letter == '\r') && (i < (MAX_TEXT_LENGTH-1)));
  text[i-1] = 0;
}

void displayCodes(void){
  uint8_t i;
  for (i=0; i<4; i++){
    transmitByte(' ');
    transmitByte('0'+i);
    printString(": ");
    printFromEEPROM(codePointers[i]);
    printString("\r\n");
  }
}

void changeCode(char codeBuffer[]){
  char input;
  char* codeAddress;
  printString(" -- Choose code phrase to replace:\r\n");
  do{
    displayCodes();
    input = receiveByte();
  } while((input < '0') || (input > '3'));
  codeAddress = codePointers[input-'0'];
  printString(" -- Enter new code: ");
  enterText(codeBuffer);
  eeprom_update_block(codeBuffer, codeAddress, CODE_LEN);
}

void selectCode(char code[]){
  char input;
  char* codeAddress;
  printFromEEPROM(promptSelectCode);
  do{
    displayCodes();
    input = receiveByte();
  } while((input < '0') || (input > '3'));
  codeAddress = codePointers[input-'0'];
  eeprom_read_block(code, codeAddress, CODE_LEN);
}

void encodeCaesar(char text[], char code[]){
  uint8_t codePosition = 0;
  uint8_t textPosition = 0;
  char thisLetter; 
  char thisCode;
  do{
    if (code[codePosition] == 0){   /* loop when at end of code phrase */
      codePosition = 0;
    }
    thisLetter = text[textPosition] - ' '; 
    thisCode = code[codePosition] - ' ';
    thisLetter += thisCode;
    if (thisLetter > 94){ 	/* keeps within printing characters */
      thisLetter -= 95;
    }
    text[textPosition] = thisLetter + ' ';
    codePosition++;		     /* and move on to the next */
    textPosition++;
  } while(text[textPosition]);	/* until zero at the end of string */
}

void decodeCaesar(char text[], char code[]){
  uint8_t codePosition = 0;
  uint8_t textPosition = 0;
  char thisLetter;
  char thisCode;
  do{
    if (code[codePosition] == 0){
      codePosition = 0;
    }
    thisLetter = text[textPosition] - ' ';    
    thisCode = code[codePosition] - ' ';
    if (thisCode > thisLetter){	/* keeps within printing chars */
      thisLetter += 95;
    }
    text[textPosition] = thisLetter - thisCode + ' '; 
    codePosition++;
    textPosition++;
  } while(text[textPosition]);
}

int main(void){
  // -------- Inits --------- //
  
  char textBuffer[MAX_TEXT_LENGTH];	
  char codeString[CODE_LEN];
  char input;
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
    printFromEEPROM(menuChangeCode);
    printFromEEPROM(menuEncode);
    printFromEEPROM(menuDecode);
    input = receiveByte();

    switch(input){
    case 'e': // encode
      encodeCaesar(textBuffer, codeString);
      break;
    case 'd': // decode
      decodeCaesar(textBuffer, codeString);
      break;
    case 'n': // new text
      printFromEEPROM(promptTypeText);
      enterText(textBuffer);
      break;
    case 'c': // choose code
      selectCode(codeString);
      break;
    case 'x': // change code
      changeCode(codeString);
      break;
    }    
  }    /* End event loop */
  return(0);                  /* This line is never reached  */
}

