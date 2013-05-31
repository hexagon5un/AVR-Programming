## Functions to convert mono, 8kHz .wav files to differential PCM

## If you've got 'sox' installed on your computer, this code calls
##  'sox' and re-codes the files for you.  If not, you've got to do it.

## Finally, given a bunch of wav files with names like one.wav, two.wav
##  this writes the DPCM data out to a header file for including in AVR C

from struct import unpack
import wave
import os
import sys

def unpackMono(waveFile):
    w = wave.Wave_read(waveFile)
    data = []
    for i in range(w.getnframes()):
        data.append(unpack("h", w.readframes(1))[0])
    return(data)

def scaleData(data):
    scale = max(max(data), abs(min(data))) * 1.0
    return([x/scale for x in data])

def getDifferences(data):
    differences = []
    for i in range(len(data)-1):
        differences.append(data[i+1]-data[i])
    return(differences)

def quantize(data, thresholds):
    quantized = []
    n = len(thresholds)
    thresholdRange = range(n)
    for d in data:
        categorized = False
        for i in thresholdRange:
            if d <= thresholds[i]:
                quantized.append(i)
                categorized = True
                break
        if not categorized:
            quantized.append(n)
    return(quantized)

def pack4(data):                # for 2-bit data
    packedData = []
    for i in range(len(data) / 4):
        thisByte = 0
        thisByte += 2**6 * data[4*i]
        thisByte += 2**4 * data[4*i+1]
        thisByte += 2**2 * data[4*i+2]
        thisByte += data[4*i+3]
        packedData.append(thisByte)
    return(packedData)

def packTwoBitDPCM(filename):
    data = unpackMono(filename)
    data = scaleData(data)
    differences = getDifferences(data)
    quantized = quantize(differences, TWO_BIT_THRESHOLDS)
    packed = pack4(quantized)
    return(packed)

def createHeader(filename, packedData):
    baseFilename = filename[:-4]
    outfile = open("DPCM_" + baseFilename + ".h", "w")
    outfile.write('uint8_t DPCM_{}[] PROGMEM = {{\n'.format(baseFilename))
    for byte in packedData:
        outfile.write('  {:d},\n'.format(byte))
    outfile.write('};\n')
    outfile.close()

def fixWaveFile(filename):
    w = wave.Wave_read(filename)
    bitrate = w.getframerate()
    channels = w.getnchannels()
    bits = w.getsampwidth()*8
    if not bitrate==8000 or not channels==1 or not bits==16:
        newFilename = filename[:-4] + "_8000.wav"
        returnValue = os.system(SOXCOMMAND.format(filename, newFilename))    
        if returnValue:
            raise(SOX_Exception("""Something went wrong calling sox: 
SOXCOMMAND.format(filename, newFilename
Is sox installed?  If not, just make sure that you've saved 8kHz mono wav files."""))
        filename = newFilename
    return(filename)
    

class SOX_Exception(Exception):
    pass

if __name__ == "__main__":

    ## Default is to convert all wav files in the current directory.
    
    TWO_BIT_THRESHOLDS = [-0.05, 0, 0.05]
    
    SOXCOMMAND = "sox {} -r 8000 -c 1 -b 16 {}" # for converting wave file
    ## install sox, or use itunes or audacity to convert 
    ## wavefile to 8kHz, 16-bit, one-channel
    
    wavefiles = [x for x in os.walk(".").next()[2] if x.endswith(".wav")]
    for filename in wavefiles:        
        filename = fixWaveFile(filename) # converts if needed, returns new filename
        packedData = packTwoBitDPCM(filename)
        createHeader(filename, packedData)
    
    ## And create a digits sample set for talkingVoltmeter.h:
    digits = ["one", "two", "three", "four", "five", "six", 
              "seven", "eight", "nine", "zero", 
              "point", "volts", "talkingvoltmeter"]
    allDigits = open("allDigits.h", "w")
    for digit in digits:
        filename = "DPCM_" + digit + "_8000.h"
        print filename
        allDigits.write(open(filename).read())
    allDigits.close()

