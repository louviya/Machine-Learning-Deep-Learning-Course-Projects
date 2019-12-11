function [param]=proj2step1()
K=48;
N=6;
filename = 'rightcamera.txt';
delimiterIn = '\t';
m = importdata(filename, delimiterIn);%load 2D data to matrix m
m=m(:,2:3); %get 2D coordinates

filename = 'modeldata.txt';
delimiterIn = '\t';
M = importdata(filename, delimiterIn);%load 3D data to matrix M
M = M(:, 2:4);

%centroid
%plot(m(:,1),m(:,2),'*');%save as mpoints.jpg
%I  =imread('mpoints.jpg');
%Ibw = im2bw(I);
%centroid = regionprops(Ibw,  'Centroid');
mx=148.0926;
my=111.6481;
s2d= 1/sqrt(mx^2+my^2);
h2d=[s2d 0 mx;0 s2d my;0 0 1];
h2dinv=inv(h2d);
for i=1:1:54
    mm(i,1)=m(i,1)*s2d+mx;
    mm(i,2)=m(i,2)*s2d+my;
end
%plot3(M(:,1),M(:,2),M(:,3),'*');%save as mpoints.jpg
center = mean(M);
Mx=11.5370;
My=10.8556;
Mz=23.5611;
s3d= 1/sqrt(Mx^2+My^2+Mz^2);
h3d=[s3d 0 0 Mx;0 s3d 0 My;0 0 s3d Mz; 0 0 0 1];
for i=1:1:54
    MM(i,1)=M(i,1)*s3d+Mx;
    MM(i,2)=M(i,2)*s3d+My;
    MM(i,3)=M(i,3)*s3d+Mz;
end

mvindex = randperm(54,N); %randomly generate 6 integers as the index of points in the validation set
mv =mm([mvindex],:); %validation set mv for 2D    extract 10 rows to mv
Mv =MM([mvindex],:); %validation set Mv for 3D
mt = mm;
mt([mvindex],:)=[]; %training set mt for 2D       extract other 62 rows to mt
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
fi = sqrt(p3(1)*p3(1)+p3(2)*p3(2)+p3(3)*p3(3));  %denominator
kesai = 1/fi; %the unknown sign alfa or kesai

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
error = sum(d)/N;


P= h2d \ [kesai*p1' kesai*p14; kesai*p2' kesai*p24;kesai*p3' kesai*p34;] * h3d;

pp1=P(1,1:3); %row vector
pp2=P(2,1:3);
pp3=P(3,1:3);
pp14=P(1,4);
pp24=P(2,4);
pp34=P(3,4);


r3 = pp3; %parameter r3
tz = pp34;
u0 = kesai*pp1*pp3';
v0 = kesai*pp2*pp3';
fu = kesai*sqrt(pp1*pp1'-u0*u0);
fv = kesai*sqrt(pp2*pp2'-v0*v0);
tx = (pp14-u0*pp34)/fu;
ty = (pp24-v0*pp34)/fv;
r1 = (pp1-u0.*pp3)/fu;
r2 = (pp2-v0.*pp3)/fv;



param =[r1 r2 r3; tx ty tz fu fv u0 v0 error 0];

end




