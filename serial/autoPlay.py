## Scripting in python to drive the serial-port organ

## So far, the "protocol" is simple.  
## Python routine sends a note, waits for a return character, then sends next, etc.
## Organ listens for notes, when it gets one sends an 'N' to say it's ready
 
import serial

def playString(noteString, serialPort):
  for letter in noteString:
    print letter
    serialPort.write(letter)
    returnValue = serialPort.read(1)
    
if __name__ == "__main__":

  import time, urllib2

  ## Need to consider alternatives for Mac / Windows
  PORT = "/dev/ttyUSB0"
  BAUD = 9600

  s = serial.Serial(PORT, BAUD)
  s.flush()                     
  ## flush clears the buffer so that we're starting fresh
  ## More on serial buffers later.

  ## An intentional example.  You can use this for playing music on purpose.
  playString("f g h j k l ; ]'[", s)
  raw_input("Press enter for next demo")

  ## A fun / stupid example.  You can just type stuff and see what comes out.
  playString("hello there, this is a random string turned into 'music'", s)
  raw_input("Press enter for next demo")

  ## A really stupid example.  Play websites!
  ## Bonus points for first person to tweet themselves a song.
  import re
  contentFilter = re.compile(r'<p>(.*?)</p>')
  songSite = urllib2.urlopen("http://serialorgansongs.jottit.com/").read()
  songText = contentFilter.findall(songSite)[0]
  playString(songText, s)
 


 

  
  
  
