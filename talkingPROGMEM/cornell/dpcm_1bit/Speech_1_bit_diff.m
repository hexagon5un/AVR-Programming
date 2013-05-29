
% Speech encoder for Mega644/GCC decoder
clear all

%===================================%the Encoder
[d,r] = wavread('mega644small.wav'); %4
%scale to about unity
res = 1/(max(max(d),abs(min(d)) ));
dd = diff(d * res);
%init the code vector
ddcode = zeros(1,length(dd));

brkpt1= 0 ;
%quantize the first derivative
ddcode(find(dd<brkpt1)) = 0;
ddcode(find(dd>=brkpt1)) = 1;

%make the length of ddcode a multiple of 8
ddcode = [ddcode,zeros(1,8-mod(length(ddcode),8))];
length(ddcode)
index=1;
for i=1:8:length(ddcode)
    packed(index)= ...
        ddcode(i)*128 + ddcode(i+1)*64 + ...
        ddcode(i+2)*32 + ddcode(i+3)*16 + ...
        ddcode(i+4)*8 + ddcode(i+5)*4 + ...
        ddcode(i+6)*2 + ddcode(i+7) ;
    index=index+1;
end
%make a textfile with GCC source code in it.
fname='mega644_1_bit.h';
fid = fopen(fname,'w');
fprintf(fid,'const prog_uint8_t mega644[%d]={\r',length(packed));
for i=1:length(packed)-1
    fprintf(fid,' %5d,\r',packed(i));
end
fprintf(fid,' %5d};\r',packed(end));
fclose(fid);

%===================================%the Decoder
%value based on quantizer
value = [-.16, .16];
dl(1)=0;
j=2;
highpassfactor = 1/8;
for i=1:length(packed)
    p1 = round(bitand(packed(i),128)/128);
    p2 = round(bitand(packed(i),64)/64);
    p3 = round(bitand(packed(i),32)/32);
    p4 = round(bitand(packed(i),16)/16);
    p5 = round(bitand(packed(i),8)/8);
    p6 = round(bitand(packed(i),4)/4);
    p7 = round(bitand(packed(i),2)/2);
    p8 = round(bitand(packed(i),1)/1);
    
    %note that a bit of highpass has been added to reduce drift
    dl(j) = dl(j-1)+ value(p1+1) - highpassfactor * dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p2+1) - highpassfactor * dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p3+1) - highpassfactor * dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p4+1) - highpassfactor * dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p5+1) - highpassfactor * dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p6+1) - highpassfactor * dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p7+1) - highpassfactor * dl(j-1);
    j=j+1;
    dl(j) = dl(j-1)+ value(p8+1) - highpassfactor * dl(j-1);
    j=j+1;
end
% dl(1)=0;
% for i=1:length(ddcode)
%     dl(i) = dl(i-1)+ value(ddcode(i)+1) - .125*dl(i-1); % .* w';
% end

%return
%====================================%playback and graphing
% sound(dl,r);
dl = dl/(max(abs(dl))+.001);
wavwrite(dl', 'mega644_1_bit.wav');

