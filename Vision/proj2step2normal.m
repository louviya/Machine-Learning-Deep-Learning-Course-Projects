function []=proj2step2normal()
%8-point
filename = 'correspondence.txt';
delimiterIn = ' ';
corres = importdata(filename, delimiterIn);%load correspondent coordinates
left = corres(:,1:2); %get left camera coordinates
right = corres(:,3:4);

[fLMedS, inliers] = estimateFundamentalMatrix(left,right,'NumTrials',2000);%get inliers
%inliers =0 1 0 0 1 1 1 0 0  0  1  0  0  1  1  0  1  1  1  1  1  0  0  0  0  0  1  1
%index    1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
%26 27 28
inliers = [2,5,6,7,11,14,15,17,18,19,20,21,27,28];
inlierPts1 = left(inliers,:);
inlierPts2 = right(inliers,:);
fNorm8Point = estimateFundamentalMatrix(inlierPts1,inlierPts2,'Method','Norm8Point');
F=fNorm8Point;
Wl=[15.4387 0 0.2328 0;0 14.3929 0.1498  0;0 0 1 0];
Wr=[15.6691 0 0.2356 0;0 14.8056 0.1690  0;0 0 1 0];
E=Wl'*F*Wr;
I1 = imread('mark_leftface.bmp');
I2 = imread('mark_rightface.bmp');
showMatchedFeatures(I1,I2,left,right,'montage','PlotOptions',{'r+','g+','y--'});

W=(Wl+Wr)/2;
tt=norm(T);
v1=T/tt;
tx=-25.3767;
ty=0.5871;
d=sqrt(tx^2+ty^2);
v2=[tx -ty 0]/d;
v3=cross(v1, v2);
Rlrect=[v1';v2;v3];
leftrect=W*Rlrect*inv(Wl)*left3';
x=leftrect(3,:);%the last row is not 1
b=diag(x);
anss=leftrect/b;
leftrect2=anss';

%testing
leftrect22=W2*Rlrect*inv(Wl)*left3';
x=leftrect22(3,:);
b=diag(x);
leftrect222=(leftrect22/b)';
