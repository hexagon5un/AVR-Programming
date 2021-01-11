import serial

def readValue(serialPort):
    return(ord(serialPort.read(1)))

def plotValue(value):
    """ Displays the value on a scaled scrolling bargraph"""
    leadingSpaces = "-" * int(value*(SCREEN_WIDTH-3) / 255)
    print(f"{leadingSpaces} {value:03}")

def cheapoScope(serialPort):
    while(1):
        newValue = readValue(serialPort)
        plotValue(newValue)
        

if __name__ == "__main__":
    ## list all serial ports being used: python -m serial.tools.list_ports
    PORT = '/dev/ttyUSB0' # update to whatever port is listed in serial.tools.list_ports
    BAUDRATE =  9600
    TIMEOUT = None
    SCREEN_WIDTH = 80

    ## Take command-line arguments to override defaults above
    import sys
    if len(sys.argv) == 3:
        port = sys.argv[1]
        baudrate = int(sys.argv[2])
    else:                        # nothing passed, use defaults 
        print ("Optional arguments port, baudrate set to defaults.")
        port, baudrate = (PORT, BAUDRATE)
        
    serialPort = serial.Serial(port, baudrate, timeout=TIMEOUT)
    serialPort.flush()
    cheapoScope(serialPort)
