clear all
%use an optimizer to find the best parameters
%for an utterance
%The encoder/decoder go into an iterative scheme to find the
%best 3 break points and 4 reconstruction values
%===================================%the Encoder
[d,r] = wavread('C:\Documents and Settings\bruce land\My Documents\Matlab\Speech\SineSynth\test4small.WAV'); %4
%scale to about unity
res = 1/(max(max(d),abs(min(d)) ));
dd = diff(d * res);

%parameters
value = [-.16, -.026 .026 .16];
brkpt1=-0.05; brkpt2=0 ; brkpt3=0.05;
%initial parameter guess
p0 = [brkpt1, brkpt2, brkpt3, ...
    value(1),value(2),value(3),value(4) ];

p = fminsearch(@FindOpt,p0,[],d,dd);
%ChiMin = fmins ('chisq', density0, options, [], smat, observed,
%fixdensity);

%=================================%print parameters
p