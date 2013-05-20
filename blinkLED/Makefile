
##########------------------------------------------------------##########
##########              Project-specific Details                ##########
##########    Check these every time you start a new project    ##########
##########------------------------------------------------------##########

MCU   = atmega168
F_CPU = 1000000

## This is where your main() routine lives
TARGET = blinkLED

## If you've split your program into multiple .c / .h files,
## include the additional files here.
EXTRA_SOURCE =

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

## List of all header files
HEADERS = $(SRC:.c=.h)

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
disassemble: $(TARGET).lst

disasm: disassemble

%.lst: %.elf
	$(OBJDUMP) -S $< > $@

# Optionally show how big the resulting program is
size:  $(TARGET).elf
	$(AVRSIZE) -C --mcu=$(MCU) $(TARGET).elf

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).obj \
	$(TARGET).o $(TARGET).d $(TARGET).eep $(TARGET).lst \
	$(TARGET).lss $(TARGET).sym $(TARGET).map $(TARGET)~

squeaky_clean:
	rm -f *.elf *.hex *.obj *.o *.d *.eep *.lst *.lss *.sym *.map *~

## Details about specific programmers are found here
include Makefile_programmers
include Makefile_fuses
