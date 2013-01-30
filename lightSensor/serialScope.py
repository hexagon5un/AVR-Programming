#!/usr/bin/env python

import serial

def readValue(serialPort):
    return(ord(serialPort.read(1)))

def plotValue(value, dashes):
    """
 	Displays the value on a scaled scrolling bargraph
    """
    value = int(value)
    if dashes:
        print "%s%3i" % (("-"*(value*77 / 255)), value)
    else:
        print "%s%3i" % ((" "*(value*77 / 255)), value)

def cheapoScope(serialPort, smoothing, dashes=True):
    runningValue = 0.0
    while(1):
        runningValue = float(readValue(serialPort)) + smoothing*runningValue
	plotValue(runningValue*(1.0 - smoothing), dashes)
        

if __name__ == "__main__":
    
    PORT = '/dev/ttyUSB0'
    BAUDRATE =  9600
    SMOOTHING = 0.1

    ## Take command-line arguments to override defaults above
    import sys
    if len(sys.argv) == 3:
        baudrate, smoothing = [float(x) for x in sys.argv[1:3]]
    else:
        baudrate, smoothing = (BAUDRATE, SMOOTHING)
        
    serialPort = serial.Serial(PORT, baudrate, timeout=10)
    serialPort.flush()
    cheapoScope(serialPort, smoothing, dashes=False)



    



  

