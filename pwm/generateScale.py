import math
scale = ['C', 'Cx', 'D', 'Dx', 'E', 'F', 'Fx', 'G', 'Gx', 'A', 'Ax', 'B']

def octave(baseLength):
    wavelengths = [baseLength / math.exp(x*math.log(2)/12) for x in range(0, 12)]
    wavelengths = [int(round(x)) for x in wavelengths]
    return( zip(scale, wavelengths) )

def octave(baseLength):
    pitches = [baseLength * math.exp(x*math.log(2)/12) for x in range(0, 12)]
    pitches = [int(round(x)) for x in pitches]
    return( zip(scale, pitches) )




for i in range(0,4):
    for note, wavelength in octave(3000 / 2**i):
        if wavelength < 30000:
            noteString = note + str(i)
            print "#define  {:<5}{:>6}".format(noteString, wavelength)
