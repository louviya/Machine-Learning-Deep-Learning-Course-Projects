function [F]=proj2step2()
%8-point
filename = 'correspondence.txt';
delimiterIn = ' ';
corres = importdata(filename, delimiterIn);%load correspondent coordinates
left = corres(:,1:2); %get left camera coordinates
right = corres(:,3:4);


%centroid
%plot(m(:,1),m(:,2),'*');%save as mpoints.jpg
%I  =imread('mpoints.jpg');
%Ibw = im2bw(I);
%centroid = regionprops(Ibw,  'Centroid');
mxl=210.75;     
myl=136.5714;
s2d= 1/sqrt(mxl^2+myl^2);
h2d=[s2d 0 mxl;0 s2d myl;0 0 1];
h2dinv=inv(h2d);
mxr=111.8929;
myr=141.8929;
s2dr= 1/sqrt(mxr^2+myr^2);
h2dr=[s2dr 0 mxr;0 s2dr myr;0 0 1];
h2drinv=inv(h2dr);
for i=1:1:28                        %normalization
    nleft(i,1)=left(i,1)*s2d+mxl;
    nleft(i,2)=left(i,2)*s2d+myl;
    nright(i,1)=right(i,1)*s2dr+mxr;
    nright(i,2)=right(i,2)*s2dr+myr;
end


N=28;
%A= zeros(N,9);
%this loop generates matrix A for Av=0
for i=1:1:N
    A(i,:)=[nleft(i,1)*nright(i,1) nleft(i,1)*nright(i,2) nleft(i,1) nleft(i,2)*nright(i,1) nleft(i,2)*nright(i,2) nleft(i,2) nright(i,1) nright(i,2) 1];
end


[U,D,V] = svd (A);%get the matrix V from svd method

f11 = V(1,9);
f12 = V(2,9);
f13 = V(3,9);
f21 = V(4,9);
f22 = V(5,9);
f23 = V(6,9);
f31 = V(7,9);
f32 = V(8,9);
f33 = V(9,9);

F1=[f11 f12 f13; f21 f22 f23; f31 f32 f33];



[Uf,Df,Vf] = svd (F1);%get the matrix Vf from svd method
Dff=Df;
Dff(3,3)=0;
F2 =Uf*Dff*Vf';
F= (h2dinv') * F2 * h2dr;%denormalization
F=F/norm(F);

%one line code
%[fLMedS, inliers] = estimateFundamentalMatrix(left,right,'NumTrials',2000);%get inliers
%inliers =0 1 0 0 1 1 1 0 0  0  1  0  0  1  1  0  1  1  1  1  1  0  0  0  0  0  1  1
%index    1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25  26 27 28
%inliers = [2,5,6,7,11,14,15,17,18,19,20,21,27,28];
%inlierPts1 = left(inliers,:);
%inlierPts2 = right(inliers,:);
%fNorm8Point = estimateFundamentalMatrix(inlierPts1,inlierPts2,'Method','Norm8Point');
%F=fNorm8Point;

