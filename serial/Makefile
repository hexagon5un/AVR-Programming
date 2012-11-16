## Stripped-down Makefile

MCU = atmega168
TARGET = organ
## If you've split your program into multiple .c / .h files, 
## include the additional source (.c) files here.
EXTRA_SOURCE = common.c


## Define programs / locations
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size

## Compilation options
CFLAGS = -g -mmcu=$(MCU) -Os -I. 
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

size:
	$(AVRSIZE) -A $(TARGET).elf

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).obj \
	$(TARGET).o $(TARGET).d $(TARGET).eep $(TARGET).lst \
	$(TARGET).lss $(TARGET).sym $(TARGET).map $(TARGET)~

super_clean:
	rm -f *.elf *.hex *.obj *.o *.d *.eep *.lst *.lss *.sym *.map *~


## Programmer-specific details here -- flashing code to AVR
flash : $(TARGET).hex
	avrdude -c $(PROGRAMMER) -p $(MCU) $(AVRDUDE_ARGS) -U flash:w:$(TARGET).hex

flash_usbtiny: PROGRAMMER = usbtiny
flash_usbtiny: AVRDUDE_ARGS =
flash_usbtiny: flash

flash_109: PROGRAMMER = avr109
flash_109: AVRDUDE_ARGS = -b 9600 -P /dev/ttyUSB0
flash_109: MCU = atmega88  # override for wrong part number in bootloader
flash_109: flash

