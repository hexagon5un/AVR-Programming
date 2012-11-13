## Stripped-down Makefile
## 

MCU = atmega168
TARGET = blinkLED
#PROGRAMMER = usbtiny
AVRDUDE_ARGS = 
#AVRDUDE_ARGS += -P /dev/ttyUSB0 
#AVRDUDE_ARGS += -b 19200 

CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=$(MCU)
OBJ2HEX=avr-objcopy

LOADER=avrdude -c $(PROGRAMMER) -p $(MCU) $(AVRDUDE_ARGS) 

all : $(TARGET).hex

flash_usbtiny: PROGRAMMER = usbtiny
flash_usbtiny: AVRDUDE_ARGS =
flash_usbtiny: flash

flash_910: PROGRAMMER = avr910
flash_910: AVRDUDE_ARGS = -P /dev/ttyUSB0 -b 115200 
flash_910: flash


flash : $(TARGET).hex
	$(LOADER) -U flash:w:$(TARGET).hex

%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

clean :
	rm -f *.hex *.obj *.o *.d *.eep *.lst *.lss *.sym *.map *~
	
