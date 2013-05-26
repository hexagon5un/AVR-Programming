#!/usr/bin/env python

import serial

class NoSyncException(Exception):
    pass

def waitForSync(serialPort, syncCodes, timeout=5):
    syncBytesReceived = 0
    for i in range(timeout):
        byte = ord(serialPort.read(1))
        if byte == syncCodes[syncBytesReceived]:
            syncBytesReceived += 1
            if syncBytesReceived == len(syncCodes):
                return("Got Sync!")
    raise NoSyncException("Failed to sync within " + str(timeout) + " bytes")

def readValues_16(serialPort, syncCodes):
    '''Reads in two bytes at a time over the serial port'''
    waitForSync(serialPort, syncCodes)
    return([ord(x) for x in serialPort.read(2)])

def plotValue(value, maxValue):
    """ Displays the value on a scaled scrolling bargraph """
    value = int(value)
    print "%s%3i" % ((" "*(value*SCREEN_WIDTH / maxValue)), value)

def cheapoScope_16(serialPort, syncCodes, maxValue):
    while(1):
        values = readValues_16(serialPort, syncCodes) # get two bytes
        value  = 256*values[0] + values[1]            # back to 16-bit number
	plotValue(value, maxValue)                    
        

if __name__ == "__main__":
    
 #   PORT = '/dev/ttyACM0'
 #   PORT = "COM3"
    PORT = '/dev/ttyUSB0'
    
    BAUDRATE =  9600
    SCREEN_WIDTH = 77
    MAX_VALUE = 1024
    SYNC_CODE = [ord("\xa5"), ord("\x5a")]

    serialPort = serial.Serial(PORT, BAUDRATE, timeout=10)
    serialPort.flush()
    cheapoScope_16(serialPort, SYNC_CODE, MAX_VALUE)
