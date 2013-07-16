

import time
import serial

def readTime(serialPort):
    '''Reads the time from the AVR over the serial port'''
    serialPort.flushInput()
    character = ""
    while(not character == "\n"):     # loop until see end of line 
        character = serialPort.read(1)
    ## The time string looks something like '011:045:023\r\n'
    timeString = serialPort.read(13) 
    hms = timeString.split(":")
    hms = [int(x) for x in hms] # make hour, minute, second numeric
    return(hms)

def setTime(serialPort, hours, minutes, seconds):
    '''Sends the time over the serial port'''
    serialPort.flushOutput()
    serialPort.write("S")
    time.sleep(0.1)             # delay while AVR sends
    serialPort.write(str(hours) + "\r")
    time.sleep(0.2)             # delay while AVR sends
    serialPort.write(str(minutes) + "\r")
    time.sleep(0.2)             # delay while AVR sends
    serialPort.write(str(seconds) + "\r")
    
def setTimeNow(serialPort):
    '''Sets the AVR clock to the current time'''
    hours, minutes, seconds = time.localtime()[3:6]    
    setTime(serialPort, hours, minutes, seconds)
    return(time.time())

def calculateTimeDelay(serialPort):
    '''Gets AVR time and subtracts off actual (computer) time'''
    avrHMS = readTime(serialPort)
    hms = time.localtime()[3:6]
    hmsDifference = [x - y for x,y in zip(avrHMS, hms)]
    out = "AVR is fast by: {x[0]} hours, {x[1]} minutes, and {x[2]} seconds"
    print out.format(x=hmsDifference)
    return(hmsDifference)

def calculateTimeDrift(serialPort, startTime):
    '''Calculates the ratio to multiply OVERFLOWS_PER_SECOND 
    given a start time and current error'''
    h, m, s = calculateTimeDelay(serialPort)
    driftSeconds = 60*60*h + 60*m + s
    elapsed = time.time() - startTime
    print "After {:.0f} seconds, ".format(elapsed)
    return (driftSeconds / elapsed + 1)
 

if __name__ == "__main__":
    
    ## Set time automatically, recording start time, 
    ##  then periodically calculate multiplication factor
    OVERFLOWS_PER_SECOND = 31250 # set this to equal the value in your code
    
    SLEEP_TIME = 10            
    ratioLog = []

    s = serial.Serial("/dev/ttyUSB0", 9600, timeout=5)
    print "Setting time to current time...."
    ratio = 0
    while not ratio == 1:       # make sure starting time is right on
        startTime = setTimeNow(s) 
        ratio = calculateTimeDrift(s, startTime)

    ## Note: you can either leave this running or 
    ## you can re-run calculateTimeDrift() at any time in the future, 
    ## as long as you don't overwrite the original startTime 
    while(True):
        ratio = calculateTimeDrift(s, startTime)
        ratioLog.append([time.time()-startTime, ratio])
        newOverflow = int(OVERFLOWS_PER_SECOND * ratio)
        print "OVERFLOWS_PER_SECOND should be {}\n\n".format(newOverflow)
        time.sleep(SLEEP_TIME)
        
    ## As you leave this routine running, you should see it bounce
    ##  around a lot in the beginning and then settle down after
    ##  running a few hours.  Ironically, it'll converge to a good
    ##  number faster if it's initially very out of sync.  (If it
    ##  drifts faster, you can figure out the drift rate sooner.)
    ## Leave it running for 24 hours and you'll get one-second-per-day 
    ##  accuracy.
