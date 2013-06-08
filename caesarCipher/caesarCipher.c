/*
Caesar Cipher encoder / decoder demo
And an excuse to play around with EEPROM memory
 */

#include "caesarCipher.h"

// -------- Functions --------- //

void printFromEEPROM(char* eepromString){
  char buffer[MAX_TEXT_LENGTH];
  eeprom_read_block(buffer, eepromString, MAX_TEXT_LENGTH);
  printString(buffer);
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

void displayCodes(char codeBuffer[]){
  uint8_t i;
  for (i=0; i<4; i++){
    eeprom_read_block(codeBuffer, codePointers[i], CODE_LEN);
    transmitByte(' ');
    transmitByte('0'+i);
    printString(": ");
    printString(codeBuffer);
    printString("\r\n");
  }
}

void changeCode(char codeBuffer[]){
  char input;
  char* codeAddress;
  printString(" -- Choose code phrase to replace:\r\n");
  do{
    displayCodes(codeBuffer);
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
    displayCodes(code);
    input = receiveByte();
  } while((input < '0') || (input > '3'));
  codeAddress = codePointers[input-'0'];
  eeprom_read_block(code, codeAddress, CODE_LEN);
}

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

