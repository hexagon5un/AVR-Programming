clear all
%reads a file at 16kHz and outputs it a 8kHz, 8 bits
[d,r] = wavread('test6.WAV');
y = resample(d,1,2);
wavwrite(y,8000,8,'test6small.wav')