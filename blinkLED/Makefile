## Stripped-down Makefile
## Works only for simple projects
## Extend for multiple .c files?


MCU = atmega168
TARGET = blinkLED


CC = avr-gcc
CFLAGS = -g -Os -Wall -mcall-prologues -mmcu=$(MCU)
OBJ2HEX = avr-objcopy


## General Makefile targets.  

all : $(TARGET).hex

flash : $(TARGET).hex
	avrdude -c $(PROGRAMMER) -p $(MCU) $(AVRDUDE_ARGS) -U flash:w:$(TARGET).hex

%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

clean :
	rm -f *.hex *.obj *.o *.d *.eep *.lst *.lss *.sym *.map *~ *.elf
	

## Programmer-specific details here
flash_usbtiny: PROGRAMMER = usbtiny
flash_usbtiny: AVRDUDE_ARGS =
flash_usbtiny: flash

flash_109: PROGRAMMER = avr109
flash_109: AVRDUDE_ARGS = -b 9600 -P /dev/ttyUSB0
flash_109: MCU = atmega88  # override for wrong part number in bootloader
flash_109: flash

