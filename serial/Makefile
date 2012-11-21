## Stripped-down Makefile

MCU = atmega168
F_CPU = 1000000

TARGET = demoOrgan
#TARGET = loopback

## If you've split your program into multiple .c / .h files, 
## include the additional source (.c) files here.
EXTRA_SOURCE = organ.c


##########------------------------------------------------------##########
##########   Stuff below here is changed very infrequently.     ##########
##########   (Once for setup, and you should be good.)          ##########
##########------------------------------------------------------##########

## Define programs / locations
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size

## Compilation options, type man avr-gcc if you're curious.
CFLAGS = -g -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os -I. 
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -std=gnu99

## Lump target and extra source files together
SRC = $(TARGET).c
SRC += $(EXTRA_SOURCE)
# For every .c file, compile an .o object file
OBJ = $(SRC:.c=.o) 

## Generic Makefile targets.  
all: $(TARGET).hex $(TARGET).lss size

%.hex: %.elf
	$(OBJCOPY) -R .eeprom -O ihex $< $@

%.elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) --output $@ 

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

size:  $(TARGET).elf
	$(AVRSIZE) -A $(TARGET).elf

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).obj \
	$(TARGET).o $(TARGET).d $(TARGET).eep $(TARGET).lst \
	$(TARGET).lss $(TARGET).sym $(TARGET).map $(TARGET)~

squeaky_clean:
	rm -f *.elf *.hex *.obj *.o *.d *.eep *.lst *.lss *.sym *.map *~


## Programmer-specific details here -- flashing code to AVR using avrdude
## If you're using another (command-line) uploader program, you can modify the
##  "flash" target below.
## If you're using a different flash programmer that supported by avrdude,
##  feel free to edit these.

flash : $(TARGET).hex
	avrdude -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -U flash:w:$(TARGET).hex

flash_usbtiny: PROGRAMMER_TYPE = usbtiny
flash_usbtiny: PROGRAMMER_ARGS =      # USBTiny works with no further arguments
flash_usbtiny: flash

flash_109: PROGRAMMER_TYPE = avr109
flash_109: PROGRAMMER_ARGS = -b 9600 -P /dev/ttyUSB0
flash_109: flash

