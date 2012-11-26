
/* 
   Some data arrays for displaying POV-style
   This part of the code should be pretty easy to extend
    and make art of your own.  
   I find that if you squint and turn your head sideways,
    you can almost make the patterns out, edit by hand.
   Alternatively, creating the byte arrays could be a cute
    Python program.   
   The way I hold my POV toy, the bottom-most LED is 
    the most-significant bit.  (Turn head right to view).    
 */


// These are code for some space invaders.  I like 'em.
// But I need more.  If you make a good one, send it to me.
uint8_t invaderData1[] = {
  0b01110000,
  0b00011000,
  0b11111101,
  0b10110110,
  0b00111100,
  0b00111100,
  0b00111100,
  0b10110110,
  0b11111101,
  0b00011000,
  0b01110000
};

uint8_t invaderData2[] = {
  0b00001110,
  0b00011000,
  0b10111101,
  0b01110110,
  0b00111100,
  0b00111100,
  0b00111100,
  0b01110110,
  0b10111101,
  0b00011000,
  0b00001110
};

// My friend Patrick Roanhouse made this in an AVR class
uint8_t Star1UP[] = {
  0b10000100,
  0b01101100,
  0b01111110,
  0b00011111,
  0b01111110,
  0b01101100,
  0b10000100,
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000010,
  0b11111111,
  0b11111111,
  0b10000000,
  0b00000000,
  0b00000000,
  0b01111111,
  0b11111111,
  0b10000000,
  0b11111111,
  0b01111111,
  0b00000000,
  0b00000000,
  0b11111111,
  0b11111111,
  0b00110011,
  0b00110011,
  0b00011110,
  0b00011110,
};
