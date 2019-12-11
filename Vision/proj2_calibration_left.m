% Calibration - left camera
% get points
leftfile = 'leftcamera.txt';
leftpoints = load(leftfile, '-ascii'); 
left_u = leftpoints(:,2);
left_v = leftpoints(:,3);

objectfile = 'modeldata.txt';
objectpoints = load(objectfile, 'ascii');
object_x = objectpoints(:,2);
object_y = objectpoints(:,3);
object_z = objectpoints(:,4);


    n = 25;
    count = 15;
    A = zeros(50, 12);
    for i = 1:2:50
        A(i,1) = object_x(count);
        A(i,2) = object_y(count);
        A(i,3) = object_z(count);
        A(i,4) = 1;
        A(i,5) = 0;
        A(i,6) = 0;
        A(i,7) = 0;
        A(i,8) = 0;
        A(i,9) = -object_x(count)*left_u(count);
        A(i,10) = -object_y(count)*left_u(count);
        A(i,11) = -object_z(count)*left_u(count);
        A(i,12) = -left_u(count);

        A(i+1,1) = 0;
        A(i+1,2) = 0;
        A(i+1,3) = 0;
        A(i+1,4) = 0;
        A(i+1,5) = object_x(count);
        A(i+1,6) = object_y(count);
        A(i+1,7) = object_z(count);
        A(i+1,8) = 1;
        A(i+1,9) = -object_x(count)*left_v(count);
        A(i+1,10) = -object_y(count)*left_v(count);
        A(i+1,11) = -object_z(count)*left_v(count);
        A(i+1,12) = -left_v(count);
        count = count+1; 
    end

    %SVD
    [U,S,V] = svd(A);
    
    v = V(:,end); %last column of V
    
    scaling_factor = 1/(sqrt(v(9)^2 + v(10)^2 + v(11)^2));
    %t_z is negative-> do not change sign of v
    %calculate proj matrix
    proj = zeros(3,4);
    count = 1;
    for i = 1:3
        for j = 1:4
           proj(i,j) = (scaling_factor*v(count));
           count = count+1;   
        end 
    end
   
    %calculate parameters
   u_o = (proj(1,1)*proj(3,1))+(proj(1,2)*proj(3,2))+(proj(1,3)*proj(3,3));
   v_o = (proj(2,1)*proj(3,1))+(proj(2,2)*proj(3,2))+(proj(2,3)*proj(3,3)); 
 
   
   f_u = sqrt(proj(1,1)^2+ proj(1,2)^2+proj(1,3)^2-(u_o^2));
   f_v = sqrt(proj(2,1)^2+ proj(2,2)^2+proj(2,3)^2-(v_o)^2);
 
   Wl = [f_u 0 u_o; 0 f_v v_o; 0 0 1];

   %calculate rotation and translation
   
   left_tx = (proj(1,4)-(u_o*proj(3,4)))/f_u;
   left_ty = (proj(2,4)-(v_o*proj(3,4)))/f_v;
   left_tz = proj(3,4);
   left_translation = [left_tx; left_ty; left_tz];
   
   left_r1 = [((proj(1,1)-u_o*proj(3,1))/f_u) ((proj(1,2)-u_o*proj(3,2))/f_u) ((proj(1,3)-u_o*proj(3,3))/f_u)];
   left_r2 = [((proj(2,1)-v_o*proj(3,1))/f_v) ((proj(2,2)-v_o*proj(3,2))/f_v) ((proj(2,3)-v_o*proj(3,3))/f_v)];
   left_r3 = [proj(3,1) proj(3,2) proj(3,3)];
   left_rotation = [left_r1; left_r2; left_r3];





