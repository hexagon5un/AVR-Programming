## This file generates headers with lookup tables for various waveforms
## Add your own.

import math

def phaseSteps(maxPhase, length=256):
    steps = range(0, length) 
    steps = [1.0*x/length * 2.0*math.pi * (maxPhase/360.0) for x in steps]
    return(steps)

def scaleAndRound(scale, data):
    data = [x*scale for x in data]
    data = [int(round(x)) for x in data]
    return(data)

def makeSin(maxPhase, scale, length=256):
    sinus = [math.sin(x) for x in phaseSteps(maxPhase, length)]
    sinus = scaleAndRound(scale, sinus)
    return(sinus)

def makeRamp(maxPhase, scale, length=256):
    ramp = [1.0 * x / length for x in range(0, length)]
    ramp = scaleAndRound(scale, ramp)
    return(ramp)

def halfTriangle(scale, length=256):
    '''Makes the first 1/2 of a triangle wave'''
    halfTri = [2.0 * x / length for x in range(0, length/2)]
    halfTri.extend(reversed([x+1.0/length for x in halfTri]))
    halfTri = scaleAndRound(scale, halfTri)
    return(halfTri)

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

    ## For symmetric, even waves  (right term?)
    datas = [("sineLUT", makeSin(90, scale=128)), 
             ("triangleLUT", makeRamp(90, scale=128))]
    writeHeader("quarterWaves.h", datas)

    ## For symmetric waves

    ## Simple bit-squashing
    twoBitTri = [x*128/3 for x in halfTriangle(scale=3)]
    threeBitTri = [x*128/7 for x in halfTriangle(scale=7)]
    fourBitTri = [x*128/15 for x in halfTriangle(scale=15)]
    
    datas = [("sineLUT", makeSin(180, scale=128)), 
             ("triangleLUT", halfTriangle(scale=128)),
             ("twoBitTriLUT", twoBitTri)
             ]
    writeHeader("halfWaves.h", datas)

    
    ## For whatevers!
    
    writeHeader("fullWaves.h", [('fullSine', [int(round(255*(0.5 + math.sin(x)/2.0))) for x in phaseSteps(360, 256)])])
    
