
#include <stdint.h>
#include <avr/pgmspace.h>
 
typedef struct {
  uint8_t offset;
  uint8_t len;
} PROGMEM char_entry;
 
#define FONT_HEIGHT 7
#define FONT_DATA_SIZE 191
 
extern char_entry font_table[128];
extern prog_uint8_t font_data[FONT_DATA_SIZE];
 
uint8_t get_char_len(uint8_t c);
uint8_t get_char_bit(uint8_t c, uint8_t row, uint8_t column);
 
