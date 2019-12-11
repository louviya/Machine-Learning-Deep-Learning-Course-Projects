function part1=proj1part1improved()
K=62;
N=10;

filename = '2Dpoints.txt';
delimiterIn = ' ';
m = importdata(filename, delimiterIn);%load 2D data to matrix m

filename = '3Dpoints_part1.txt';
delimiterIn = ' ';
M = importdata(filename, delimiterIn);%load 3D data to matrix M

index = randperm(72,N); %randomly generate 10 integers as the index of points in the validation set
%59    65     9    64    44     7    19    36    62    61
mv =m([index],:); %validation set mv for 2D
Mv =M([index(1) index(2) index(3) index(4) index(5) index(6) index(7) index(8) index(9) index(10)],:); %validation set Mv for 3D
mt = m;
mt([index(1) index(2) index(3) index(4) index(5) index(6) index(7) index(8) index(9) index(10)],:)=[]; %training set mt for 2D
Mt = M;
Mt([index(1) index(2) index(3) index(4) index(5) index(6) index(7) index(8) index(9) index(10)],:)=[]; %training set Mt FOR 3D

A= zeros(2*K,12);
j=1;     %this loop generates matrix A for Av=0
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

%mest =zeros(N, 2);
%d =zeros(N, 2);
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

part1= [p1'; p2';p3'; kesai tz u0; v0 tx ty; fu fv 0;r1';r2';r3';error 0 0];