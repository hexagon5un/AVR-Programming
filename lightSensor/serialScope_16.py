#!/usr/bin/env python

import serial

def readValues(serialPort):
    '''Reads in two bytes at a time over the serial port'''
    return([ord(x) for x in serialPort.read(2)])

def plotValue(value, maxValue, dashes):
    """
 	Displays the value on a scaled scrolling bargraph
    """
    value = int(value)
    if dashes:
        print "%s%3i" % (("-"*(value*SCREEN_WIDTH / maxValue)), value)
    else:
        print "%s%3i" % ((" "*(value*SCREEN_WIDTH / maxValue)), value)

def cheapoScope_16(serialPort, maxValue, dashes=True):
    while(1):
        values = readValues(serialPort)
        value  = 256*values[0] + values[1]
	plotValue(value, maxValue, dashes)
        

if __name__ == "__main__":
    
    PORT = '/dev/ttyUSB0'
    BAUDRATE =  9600
    SCREEN_WIDTH = 77
    MAX_VALUE = 2400

    serialPort = serial.Serial(PORT, BAUDRATE, timeout=10)
    serialPort.flush()

    cheapoScope_16(serialPort, MAX_VALUE, dashes=True)



    



  

