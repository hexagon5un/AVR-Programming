import serial
import calibrateTime
s=serial.Serial("/dev/ttyUSB0", 9600, timeout=5)
calibrateTime.setTimeNow(s)
s.close()
