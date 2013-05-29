
% Speech encoder for Mega644/GCC decoder
clear all

%===================================%the Encoder
[d,r] = wavread('Z:\EEdocs\4760\ece4760labstuff\GCCmega644\Speech\AllDigits8khz.WAV'); %4
%scale to about unity
res = 1/(max(max(d),abs(min(d)) ));
dd = diff(d * res);
%init the code vector
ddcode = zeros(1,length(dd));

% brkpt1=-0.5; brkpt2=0 ; brkpt3=0.5;
% value = [-.75, -.25 .25 .75];
%breakpoints are changeable
brkpt1=-0.05; brkpt2=0 ; brkpt3=0.05;
%quantize the first derivative
ddcode(find(dd<brkpt1))=0;
ddcode(find(dd>=brkpt1 & dd<brkpt2))=1;
ddcode(find(dd>=brkpt2 & dd<brkpt3))=2;
ddcode(find(dd>=brkpt3))=3;

%make the length of ddcode a multiple of 4
ddcode = [ddcode,zeros(1,4-mod(length(ddcode),4))];
length(ddcode)
index=1;
for i=1:4:length(ddcode)
    packed(index)=ddcode(i)*64 + ddcode(i+1)*16 + ...
        ddcode(i+2)*4 + ddcode(i+3) ;
    index=index+1;
end
%make a textfile with GCC source code in it.
fname='DPCMAllDigits.h';
fid = fopen(fname,'w');
fprintf(fid,'const prog_uint8_t DPCMAllDigits[%d]={\r',length(packed));
for i=1:length(packed)-1
    fprintf(fid,' %5d,\r',packed(i));
end
fprintf(fid,' %5d};\r',packed(end));
fclose(fid);

%===================================%the Decoder
%value based on quantizer
value = [-.16, -.026 .026 .16];
dl(1)=0;
j=2;
for i=1:length(packed)
    p1 = fix(bitand(packed(i),192)/64);
    p2 = fix(bitand(packed(i),48)/16);
    p3 = fix(bitand(packed(i), 12)/4);
    p4 = fix(bitand(packed(i),3));
    %note that a bit of highpass has been added to reduce drift
    dl(j) = dl(j-1)+ value(p1+1) - .125*dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p2+1) - .125*dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p3+1) - .125*dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p4+1) - .125*dl(j-1);
    j=j+1;
end
% dl(1)=0;
% for i=1:length(ddcode)
%     dl(i) = dl(i-1)+ value(ddcode(i)+1) - .125*dl(i-1); % .* w';
% end

%====================================%playback and graphing
soundsc(dl,r);
% Compare the spectrograms
figure(1);clf

subplot(211)
spectrogram(d,256,r);
colormap gray
title('Original');

subplot(212)
spectrogram(dl,256,r);
colormap gray
title('synth');



