
import time
import serial



def readTime(serialPort):
    serialPort.flushInput()
    character = "A"
    while(not character == "\n"):                # loop until see end of line characters
        character = serialPort.read(1)
    timeString = serialPort.read(13)
    hms = timeString.split(":")
    hms = [int(x) for x in hms]
    return(hms)

def setTime(serialPort, hours, minutes, seconds):
    serialPort.flushOutput()
    serialPort.write("S")
    time.sleep(0.1)             # delay while AVR sends
    serialPort.write(str(hours) + "\r")
    time.sleep(0.1)             # delay while AVR sends
    serialPort.write(str(minutes) + "\r")
    time.sleep(0.1)             # delay while AVR sends
    serialPort.write(str(seconds) + "\r")
    
def syncTimeNow(serialPort):
    '''Sets the AVR clock to the current time, taking time-setting delay into account'''
    hours, minutes, seconds = time.localtime()[3:6]
    setTime(serialPort, hours, minutes, seconds)
    return(time.time())

def calculateTimeDelay(serialPort):
    avrHMS = readTime(serialPort)
    hms =    time.localtime()[3:6]
    hmsDifference = [x - y for x,y in zip(avrHMS, hms)]
    print "AVR is fast by: {x[0]} hours, {x[1]} minutes, and {x[2]} seconds".format(x=hmsDifference)
    return(hmsDifference)

def calculateTimeDrift(serialPort, startTime):
    elapsed = time.time() - startTime
    print "After {:.0f} seconds, ".format(elapsed)
    h, m, s = calculateTimeDelay(serialPort)
    driftSeconds = 60*60*h + 60*m + s
    return (driftSeconds / elapsed + 1)
 

if __name__ == "__main__":
    
    ## Set time automatically, recording start time, then calculate multiplication factor
    OVERFLOWS_PER_SECOND = 31250 # set this to equal the value in your code
    SLEEP_TIME = 10            
    ratioLog = []

    s = serial.Serial("/dev/ttyUSB0", 9600, timeout=5)
    print "Setting time to current time...."
    startTime = syncTimeNow(s)

    ## Now wait a long time, 
    ## or just re-run calculateTimeDrift(s, startTime) later
    print "Sleeping, come back in {} seconds...".format(SLEEP_TIME)

    ## Note: you can either leave this running or 
    ## you can re-run calculateTimeDrift() at any time in the future, 
    ## as long as you don't overwrite the original startTime 
    while(True):
        ratio = calculateTimeDrift(s, startTime)
        ratioLog.append([time.time()-startTime, ratio])
        print "Your new OVERFLOWS_PER_SECOND value should be {}".format(int(OVERFLOWS_PER_SECOND * ratio))
        time.sleep(SLEEP_TIME)
        
    ## Unfortunately, the way I do timing here with the serial port is simple,
    ##  and only good to within a second, so this isn't going to be very accurate 
    ##  unless you let it run for a _long_ time.
    ## As you leave it running, though, you should see that it bounces around a while
    ##  and then settles on a number or two.  You can use this value.
    ## After all, the sundial application only updates once per minute, 
    ##  and the servo's resolution is more like one minute if you display 10 hours.
    ## If you've changed OVERFLOWS_PER_SECOND in your C code, don't forget to change it here.
    
