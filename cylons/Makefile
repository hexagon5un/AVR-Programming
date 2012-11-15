## Stripped-down Makefile
## Works only for simple projects
## Extend for multiple .c files?


MCU = atmega168
TARGET = cylonEyes_halfStepping

## Define these for your system, standard options

CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -g -Os -Wall -mcall-prologues -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
OBJ2HEX = avr-objcopy

## General Makefile targets.  

all : $(TARGET).hex

flash : $(TARGET).hex
	avrdude -c $(PROGRAMMER) -p $(MCU) $(AVRDUDE_ARGS) -U flash:w:$(TARGET).hex

%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).obj \
	$(TARGET).o $(TARGET).d $(TARGET).eep $(TARGET).lst \
	$(TARGET).lss $(TARGET).sym $(TARGET).map $(TARGET)~

super_clean:
	rm -f *.elf *.hex *.obj *.o *.d *.eep *.lst *.lss *.sym *.map *~
	

## Programmer-specific details here
flash_usbtiny: PROGRAMMER = usbtiny
flash_usbtiny: AVRDUDE_ARGS =
flash_usbtiny: flash

flash_109: PROGRAMMER = avr109
flash_109: AVRDUDE_ARGS = -b 9600 -P /dev/ttyUSB0
flash_109: MCU = atmega88  # override for wrong part number in bootloader
flash_109: flash

