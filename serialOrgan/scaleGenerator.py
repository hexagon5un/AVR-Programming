import math

    
SCALE = ['C', 'Cx', 'D', 'Dx', 'E', 'F', 'Fx', 'G', 'Gx', 'A', 'Ax', 'B']

def calculateOctave(baseLength):
    wavelengths = [baseLength / math.exp(x*math.log(2)/12) for x in range(0, 12)]
    wavelengths = [int(round(x)) for x in wavelengths]
    return( zip(SCALE, wavelengths) )

def makePitches(base):
    pitchList = []
    for octave in range(0,4):
        for note, wavelength in calculateOctave(base / 2**octave):
            if wavelength < 65500:
                noteString = note + str(octave)
                pitchList.append((noteString,wavelength))
    return(pitchList)            
    
def makeDefines(base):
    pitchList = makePitches(base)
    defineString = ""
    for (note, length) in pitchList:
        defineString += "#define  {:<5}{:>6}\n".format(note, length)
    return(defineString)

if __name__ == "__main__":

    OUTFILE = "scale16.h"
    out = open(OUTFILE, "w")
    out.write(makeDefines(50000))
    out.close()

    


