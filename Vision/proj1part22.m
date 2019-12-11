function [P, errorp]=proj1part22(mvindex)
K=62;
N=10;

filename = '2Dpoints.txt';
delimiterIn = ' ';
m = importdata(filename, delimiterIn);%load 2D data to matrix m

filename = '3Dpoints_part2.txt';
delimiterIn = ' ';
M = importdata(filename, delimiterIn);%load 3D data to matrix M

%centroid
%plot(m(:,1),m(:,2),'*');%save as mpoints.jpg
I  =imread('mpoints.jpg');
Ibw = im2bw(I);
centroid = regionprops(Ibw,  'Centroid');
mx=280.8281;
my=210.0131;
s2d= 1/sqrt(mx^2+my^2);
h2d=[s2d 0 mx;0 s2d my;0 0 1];
h2dinv=inv(h2d);
for i=1:1:72
    mm(i,1)=m(i,1)*s2d+mx;
    mm(i,2)=m(i,2)*s2d+my;
end
%plot3(M(:,1),M(:,2),M(:,3),'*');%save as mpoints.jpg
center = mean(M);
Mx=77;
My=77;
Mz=130.5833;
s3d= 1/sqrt(Mx^2+My^2+Mz^2);
h3d=[s3d 0 0 Mx;0 s3d 0 My;0 0 s3d Mz; 0 0 0 1];
for i=1:1:72
    MM(i,1)=M(i,1)*s3d+Mx;
    MM(i,2)=M(i,2)*s3d+My;
    MM(i,3)=M(i,3)*s3d+Mz;
end


mv =mm([mvindex],:); %validation set mv for 2D
Mv =MM([mvindex],:); %validation set Mv for 3D
mt = mm;
mt([mvindex],:)=[]; %training set mt for 2D
Mt = MM;
Mt([mvindex],:)=[]; %training set Mt FOR 3D

A= zeros(2*K,12);
j=1;     %this loop generates matrix A for Av=0
t=zeros(1,3);
for i=1:K
    t = [Mt(i, 1) Mt(i, 2) Mt(i, 3)].*mt(i,1);
    t2= [Mt(i, 1) Mt(i, 2) Mt(i, 3)].*mt(i,2);
    A(j, :)=[Mt(i, 1) Mt(i, 2) Mt(i, 3) 1 0 0 0 0 -t -mt(i, 1)];
    A(j+1, :)=[0 0 0 0 Mt(i, 1) Mt(i, 2) Mt(i, 3) 1 -t2 -mt(i, 2)];
    j=j+2;
    if (j==(2*K-1)) 
        break; 
    end
end


[U,S,V] = svd (A);%get the matrix V from svd method

p34 = V(12,12);
p14 = V(4,12);
p24 = V(8,12);
p1 = [V(1,12);V(2,12);V(3,12)];
p2 = [V(5,12);V(6,12);V(7,12)];
p3 = [V(9,12);V(10,12);V(11,12)];%from the last column of V
fi = sqrt(p3(1)*p3(1)+p3(2)*p3(2)+p3(3)*p3(3));
kesai = 1/fi; %the unknown sign alfa or kesai
r3 = kesai.*p3; %parameter r3
tz = kesai*p34;
u0 = p1'*p3;
v0 = p2'*p3;
fu = sqrt(p1'*p1-u0*u0);
fv = sqrt(p2'*p2-v0*v0);
tx = kesai*(p14-u0*p34)/fu;
ty = kesai*(p24-v0*p34)/fv;
r1 = kesai*(p1-u0.*p3)/fu;
r2 = kesai*(p2-v0.*p3)/fv;

P= h2d \ [kesai*p1' kesai*p14; kesai*p2' kesai*p24;kesai*p3' kesai*p34;] * h3d;
for i=1:1:N       %recover 2D points from P 
    labda = p3'* [Mv(i,1) Mv(i,2) Mv(i,3)]'+p34;
    mest(i, 1)= (1/labda).* (p1'* [Mv(i,1) Mv(i,2) Mv(i,3)]'+p14);
    mest(i, 2)= (1/labda).* (p2'* [Mv(i,1) Mv(i,2) Mv(i,3)]'+p24);
end
difference = mv-mest;
for i=1:N
    d(i)=sqrt(difference(i,1)*difference(i,1)+difference(i,2)*difference(i,2));
end
sum(d);
errorp = sum(d)/N;



end




