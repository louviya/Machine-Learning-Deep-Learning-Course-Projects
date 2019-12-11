

I11=imread('leftface.jpg');
image(I11)
hold on
filename = 'correspondence2.txt';
delimiterIn = ' ';
corres = importdata(filename, delimiterIn);%load correspondent coordinates
left = corres(:,1:2); %get left camera coordinates
right = corres(:,3:4);
one=ones(28,1);
left3=[left one];%got the three demension
right3=[right one];


leftrectadjust=Wadjust*Rlrect\(Wl)*left3';
leftrectadjust=leftrectadjust'; %got last row become 1
plot(leftrectadjust(:,1)*(-1),leftrectadjust(:,2), 'r*')

I22=imread('rightface.jpg');
figure;
image(I22)
hold on
rightrectadjust=Wadjust.*Rrrect\(Wr)*right3';
rightrectadjust=rightrectadjust'; %got last row become 1
plot(rightrectadjust(:,1)*(-1),rightrectadjust(:,2), 'r*')
