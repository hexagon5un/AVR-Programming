#!/usr/bin/env python
 
from PIL import Image
from optparse import OptionParser
from string import Template
import sys
 
# pngToFont takes a simple 1-bit image of a pixel font
# and converts it to a chunk of C code that we can use
# in microcontroller code.
 
# Edit this to reflect the order that characters appear
# in your font.  Each character should be seperated by
# one column of white pixels.
char_order = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.!?@/:;()"
 
# Header and types for font.
# We're a little tight on space, so it's program memory
# for you.  Hooray for Harvard architectures.
font_header_templ = Template("""
#include <stdint.h>
#include <avr/pgmspace.h>
 
typedef struct {
  uint8_t offset;
  uint8_t len;
} PROGMEM char_entry;
 
#define FONT_HEIGHT $height
#define FONT_DATA_SIZE $data_size
 
extern char_entry font_table[128];
extern prog_uint8_t font_data[FONT_DATA_SIZE];
 
uint8_t get_char_len(uint8_t c);
uint8_t get_char_bit(uint8_t c, uint8_t row, uint8_t column);
 
""")
 
font_source_templ = Template("""
#include "$header"
 
char_entry font_table[128] = {
$font_table
};
 
prog_uint8_t font_data[FONT_DATA_SIZE] = {
$font_data
};
 
uint8_t get_char_len(uint8_t c) {
  return pgm_read_byte(&(font_table[c].len));
}
 
uint8_t get_char_bit(uint8_t c, uint8_t row, uint8_t column) {
  uint8_t offset = pgm_read_byte(&(font_table[c].offset));
  uint8_t col = pgm_read_byte(&(font_data[offset+column]));
  return ((col & _BV(row)) == 0)?0:1;
}
 
""")
 
def get_column(im,x_offset):
    "Get the bitmap column as an int, with black pixels as 1"
    (_, height) = im.size
    val = 0
    for bit in range(height):
        if im.getpixel((x_offset,bit)) == 0:
            val = val | (1 << bit)
    return val
 
class Character:
    def __init__(self,im,x_offset):
        self.len = 0
        self.columns = []
        column = get_column(im,x_offset)
        while column != 0:
            self.columns.append(column)
            self.len = self.len + 1
            x_offset = x_offset + 1
            column = get_column(im,x_offset)
 
charmap = {}
 
def png_to_font(im,c_path,h_path):
    c_file = open(c_path,"w")
    h_file = open(h_path,"w")
    x_offset = 0
    data_size = 0
    # load all the characters from the image file
    for char in char_order:
        charmap[char] = Character(im,x_offset)
        x_offset = x_offset + charmap[char].len + 1
        data_size = data_size + charmap[char].len
        print "Char " + char + " is len " + str(charmap[char].len)
    # write out the header
    h_file.write(font_header_templ.substitute(
            count=len(char_order),
            data_size=data_size,
            height=im.size[1]))
    h_file.close
    # build the tables
    font_data = ""
    font_table = ""
    offset = 0
    for i in range(128):
        char = chr(i)
        if charmap.has_key(char):
            c = charmap[char]
            c.offset = offset
            font_table = font_table + "  {%d, %d},\n" % (offset,c.len)
            font_data = font_data + ",\n".join(map(hex,c.columns)) + ",\n"
            offset = offset + c.len
        else:
            font_table = font_table + "  {0,0},\n"
    # write out the source
    c_file.write(font_source_templ.substitute(
            font_table = font_table,
            font_data = font_data,
            header=h_path))
    c_file.close()
 
 
def main():   
    parser = OptionParser(usage="usage: %prog [options] source")
    parser.add_option("-o","--output",dest="out_path",
                      help="output to given base path")
    (options,args) = parser.parse_args()
    if len(args) != 1:
        parser.error("Please provide a single input file.")
    image = Image.open(args[0])
    image = image.convert("L")
    c_path = "font.c"
    h_path = "font.h"
    if (options.out_path):
        c_path = options.out_path + ".c"
        h_path = options.out_path + ".h"
    png_to_font(image,c_path,h_path)
 
if __name__ == "__main__":
    main()
