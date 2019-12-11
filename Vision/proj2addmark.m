mlf=imread('mark_leftface.bmp');
imshow(mlf);
hold on
x=[214, 204, 178, 205];
y=[158,139,122,200];
plot(x, y, 'yo');

figure;
mrf=imread('mark_rightface.bmp');
imshow(mrf);
hold on
xx=[112, 102, 83, 112];
yy=[163, 142, 127, 207];
plot(xx, yy, 'yo');


d=sum(left);
center=d/28;