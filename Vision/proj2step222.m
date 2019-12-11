function [param]=proj2step2()


Rl=[0.4159 -0.9083 -0.0416;-0.0516  -0.1208 -0.9913;-0.0223 -0.0265 -0.0061];
Rr=[0.1815 -0.9817 -0.0554; -0.0928 -0.1006 -0.9905;-0.0281 -0.0201 -0.0068];
Tl=[-25.304;-0.3950;-2.5726];
Tr=[-20.162;-1.4574;-2.6389];
R=Rl*Rr';
T=Tl-R*Tr;
S=[0 2.1040 -4.3981;-2.1040 0 5.6025;-4.3981 -5.6025 0];
E=S'*R;
Wl=[15.4387 0 0.2328 0;0 14.3929 0.1498  0;0 0 1 0];
Wr=[15.6691 0 0.2356 0;0 14.8056 0.1690  0;0 0 1 0];
W=(Wl+Wr)/2;
W=W(:, 1:3);
tt=norm(T);
v1=T/tt;
tx=-5.602517255688003;
ty=4.398138292405999;
d=sqrt(tx^2+ty^2);
v2=[tx -ty 0]/d;
v3=cross(v1, v2);
Rlrect=[v1';v2;v3];
recleft=W*Rlrect*inv(Wl)*left;

leftrectadjust=leftrectfinal-aa;
leftrectadjust=abs(leftrectadjust);
plot(leftrectadjust(:,1),leftrectadjust(:,2), 'r*')