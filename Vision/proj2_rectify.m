filename = 'correspondence2.txt';
delimiterIn = ' ';
corres = importdata(filename, delimiterIn);%load correspondent coordinates
left = corres(:,1:2); %get left camera coordinates
right = corres(:,3:4);
one=ones(28,1);
left3=[left one];%got the three demension
right3=[right one];
Wrect=(Wl+Wr)/2;

I11=imread('mark_leftface.bmp');
image(I11)
hold on
Rleft=left_rotation;
Rright=right_rotation;
R=Rleft*Rright';
Tl=left_translation;
Tr=right_translation;
T=Tl-R*Tr;
v1=T/norm(T);
v2=cross(v1,[0 0 1]');
v3=cross(v1,v2);
Rlrect=[v1';v2';v3'];
leftrect=Wrect*Rlrect\(Wl)*left3';
x=leftrect(3,:);%the last row is not 1
b=diag(x);
leftrectfinal=leftrect'; %got last row become 1
plot(leftrectfinal(:,1)*(-1)-bb,leftrectfinal(:,2), 'r*')
axis([-350 360 -150 240]);

I22=imread('mark_rightface.bmp');
figure;
image(I22)
hold on
Rrrect=Rlrect*R;
rightrect=Wrect.*Rrrect\(Wr)*right3';
x=leftrect(3,:);%the last row is not 1
b=diag(x);
rightrectfinal=rightrect'; %got last row become 1
plot(rightrectfinal(:,1)*(-1)+bb,rightrectfinal(:,2), 'r*')
axis([0 710 -150 240]);