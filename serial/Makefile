## Stripped-down Makefile


MCU = atmega168
TARGET = organ
## If you've split your program into multiple .c / .h files, need to include
##  the additional object files to make here.
OTHER_OBJECTS = common.o foo.o

## Compilation options
CC = avr-gcc
CFLAGS = -g -Os -Wall -mcall-prologues -mmcu=$(MCU) 
OBJ2HEX = avr-objcopy

## Generic Makefile targets.  
all : $(TARGET).hex

%.obj : %.o $(OTHER_OBJECTS)
	$(CC) $(CFLAGS) $< -o $@ $(OTHER_OBJECTS)

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

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

