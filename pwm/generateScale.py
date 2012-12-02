import math
scale = ['C', 'Cx', 'D', 'Dx', 'E', 'F', 'Fx', 'G', 'Gx', 'A', 'Ax', 'B']
basePitch = 130
octaves = 5

def octave(baseLength):
    pitches = [baseLength * math.exp(x*math.log(2)/12) for x in range(0, 12)]
    pitches = [int(round(x)) for x in pitches]
    return( zip(scale, pitches) )


outfile = open("scale.h", "w")

outfile.write('''/*

Scales for use with DDS synthesis.  

Aimed roughly at having A2 be at 440Hz, 
when the chip is clocked at 8MHz and
using 8-bit resolution on the PWM.

Tune it if you'd like.

*/

''')

for i in range(0, octaves):
    for note, pitch in octave(basePitch * 2**i):
        if pitch < 30000:
            noteString = note + str(i)
            print("#define  {:<5}{:>6}").format(noteString, pitch)
            outfile.write("#define  {:<5}{:>6}\n".format(noteString, pitch))

outfile.close()

