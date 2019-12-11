function [F]=proj2step2withoutnorm()
%8-point
filename = 'correspondence.txt';
delimiterIn = ' ';
corres = importdata(filename, delimiterIn);%load correspondent coordinates
left = corres(:,1:2); %get left camera coordinates
right = corres(:,3:4);



N=28;
%A= zeros(N,9);
%this loop generates matrix A for Av=0
for i=1:1:N
    A(i,:)=[left(i,1)*right(i,1) left(i,1)*right(i,2) left(i,1) left(i,2)*right(i,1) left(i,2)*right(i,2) left(i,2) right(i,1) right(i,2) 1];
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
F= F2;  %denormalization
