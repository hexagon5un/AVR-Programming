function fit = FindOpt(p,d,dd)
p
%init the code vector
ddcode = zeros(1,length(dd));
%quantize the first derivative
ddcode(find(dd<p(1)))=0;
ddcode(find(dd>=p(1) & dd<p(2)))=1;
ddcode(find(dd>=p(2) & dd<p(3)))=2;
ddcode(find(dd>=p(3)))=3;

dl = zeros(1,length(ddcode));
for i=2:length(ddcode)
    dl(i) = dl(i-1)+ p(ddcode(i)+4) - .125*dl(i-1); % .* w';
end

fit = mean((d(2:end)-dl').^2);