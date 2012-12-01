## This file generates headers with lookup tables for various waveforms
## Add your own.

import math

def phaseSteps(maxPhase, length=256):
    steps = range(0, length) 
    steps = [1.0*x/length * 2.0*math.pi * (maxPhase/360.0) for x in steps]
    return(steps)

def scaleAndRound(data, scale):
    data = [0.0+x-min(data) for x in data]
    data = [1.0*x/max(data)*scale for x in data]
    data = [int(round(x)) for x in data]
    return(data)

def makeSin(maxPhase, length=256):
    sinus = [math.sin(x) for x in phaseSteps(maxPhase, length)]
    return(sinus)

def prettyPrint(numList, perLine = 8):
    outString = ""
    for i in range(0, (len(numList)) / perLine):
        strings = [str(x) for x in numList[perLine*i:(perLine*i+perLine)]]
        outString += "\t" + ", ".join(strings) + ",\n"
    outString = outString[:-2] + "\n" # drop the final comma
    return(outString)

def writeHeader(fileName, dataList):
    ''' DataList is a list of pairs: [(dataName, data), ...] that include
        the data you'd like written out to the header.
    '''
    outfile = open(fileName, "w")
    for dataName, data in dataList:
        outfile.write("uint8_t {}[{:d}] = {{ \n".format(dataName, len(data)))
        outfile.write(prettyPrint(data))
        outfile.write("};\n")
    outfile.close()



if __name__ == "__main__":
    
    ## Full-waves, full 256 bytes, 0-255 range
    sinWave = scaleAndRound(makeSin(360), 255)

    triangleWave = range(0,64)
    triangleWave.extend(range(64, -64, -1))
    triangleWave.extend(range(-64, 0, 1))
    triangleWave = scaleAndRound(triangleWave, 255)

    sawWave = scaleAndRound(range(0,256), 255)

    squareWave = [0]*128
    squareWave.extend([1]*128)
    squareWave = scaleAndRound(squareWave, 255)

    writeHeader("fullWaves.h", [('fullSine', sinWave),
                                ('fullTriangle', triangleWave),
                                ('fullSaw', sawWave),
                                ('fullSquare', squareWave),
                                ])
    
    
