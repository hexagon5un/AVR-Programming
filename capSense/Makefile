
##########------------------------------------------------------##########
##########              Project-specific Details                ##########
##########    Check these every time you start a new project    ##########
##########------------------------------------------------------##########

MCU   = atmega168
F_CPU = 8000000

## This is where your main() routine lives
TARGET = main

## If you've split your program into multiple .c / .h files, 
## include the additional files here.
EXTRA_SOURCE = USART

##########------------------------------------------------------##########
##########                 Programmer Defaults                  ##########
##########          Set up once, then forget about it           ##########
##########        (Can override.  See bottom of file.)          ##########
##########------------------------------------------------------##########

PROGRAMMER_TYPE = usbtiny
# extra arguments to avrdude: baud rate, chip type, -F flag, etc.
PROGRAMMER_ARGS = 	


##########------------------------------------------------------##########
##########                   Makefile Magic!                    ##########
##########         Summary:                                     ##########
##########             We want a .hex file                      ##########
##########             Make .hex from .elf                      ##########
##########             .elf file needs all the .o objects,      ##########
##########               which get compiled from .c files       ##########
##########------------------------------------------------------##########

## Defined programs / locations
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
AVRDUDE = avrdude

## Compilation options, type man avr-gcc if you're curious.
CFLAGS = -g -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os -I. 
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -std=gnu99

## Lump target and extra source files together
SRC = $(TARGET).c
SRC += $(addsuffix .c, $(EXTRA_SOURCE)) 

## For every .c file, compile an .o object file
OBJ = $(SRC:.c=.o) 

## Generic Makefile targets.  (Only .hex file is necessary)
all: $(TARGET).hex 

%.hex: %.elf
	$(OBJCOPY) -R .eeprom -O ihex $< $@

%.elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) --output $@ 

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Optionally create listing file from .elf
# This creates approximate assembly-language equivalent of your code.
# Useful for debugging time-sensitive bits, 
# or making sure the compiler does what you want.
%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

# Optionally show how big the resulting program is 
size:  $(TARGET).elf
	$(AVRSIZE) -C --mcu=$(MCU) $(TARGET).elf

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).obj \
	$(TARGET).o $(TARGET).d $(TARGET).eep $(TARGET).lst \
	$(TARGET).lss $(TARGET).sym $(TARGET).map $(TARGET)~

squeaky_clean:
	rm -f *.elf *.hex *.obj *.o *.d *.eep *.lst *.lss *.sym *.map *~


##########------------------------------------------------------##########
##########              Programmer-specific details             ##########
##########           Flashing code to AVR using avrdude         ##########
##########                                                      ##########
##########      If you're using another uploader program,       ##########
##########           much of this will not work for you.        ##########
##########------------------------------------------------------##########

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -U flash:w:$(TARGET).hex

## If you've got multiple programmers that you use, 
## you can define them here so that it's easy to switch
flash_usbtiny: PROGRAMMER_TYPE = usbtiny
flash_usbtiny: PROGRAMMER_ARGS =  # USBTiny works with no further arguments
flash_usbtiny: flash

flash_109: PROGRAMMER_TYPE = avr109
flash_109: PROGRAMMER_ARGS = -b 9600 -P /dev/ttyUSB0
flash_109: flash


##########------------------------------------------------------##########
##########       Fuse settings and suitable defaults            ##########
##########------------------------------------------------------##########

## Mega 48, 88, 168, 328 default values
LFUSE = 0x62
HFUSE = 0xdf
EFUSE = 0x00

## Generic 
FUSE_STRING = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m 

fuses: 
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) \
	           $(PROGRAMMER_ARGS) $(FUSE_STRING)

show_fuses:
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -nv	

## Called with no extra definitions, sets to defaults
set_default_fuses:  fuses

## Set the fuse byte for turbo mode
set_fast_fuse: LFUSE = 0xE2
set_fast_fuse: fuses
