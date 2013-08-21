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
    SMOOTHING = 0
    TIMEOUT = None

    ## Take command-line arguments to override defaults above
    import sys
    if len(sys.argv) == 4:
        port = sys.argv[1]
        baudrate, smoothing = [float(x) for x in sys.argv[2:4]]
    else:
        port, baudrate, smoothing = (PORT, BAUDRATE, SMOOTHING)
        
    serialPort = serial.Serial(port, baudrate, timeout=TIMEOUT)
    serialPort.flush()
    cheapoScope(serialPort, smoothing, dashes=False)



    



  

