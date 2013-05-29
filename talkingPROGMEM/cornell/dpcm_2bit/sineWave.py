## Demo python program to create sine wave

from struct import pack, unpack
from math import sin, pi
import wave
import os
import random


RATE = 44100

## GENERATE MONO FILE ##
wv = wave.open('test_mono.wav', 'w')
wv.setparams((1, 2, RATE, 0, 'NONE', 'not compressed'))
maxVol=2**15-1.0 #maximum amplitude
wvData=""
for i in range(0, RATE*3):
    wvData+=pack('h', maxVol*sin(2*pi*i*440.0/RATE))

wv.writeframes(wvData)
wv.close()
os.system("mplayer test_mono.wav")

## GENERATE STEREO FILE ##
wv = wave.open('test_stereo.wav', 'w')
wv.setparams((2, 2, RATE, 0, 'NONE', 'not compressed'))
maxVol=2**15-1.0 #maximum amplitude
waveData = ""
for i in range(0, RATE*3):
    t = 2*pi*i/RATE             # time-step in radians*sec
    waveData+=pack('h', maxVol*sin(t*440.0)) #440Hz left
    waveData+=pack('h', maxVol*sin(t*220.0)) #220Hz right
    

wv.writeframes(waveData)
wv.close()
os.system("mplayer test_stereo.wav")

