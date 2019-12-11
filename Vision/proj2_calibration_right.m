% Calibration - right camera
% get points
rightfile = 'rightcamera.txt';
rightpoints = load(rightfile, 'ascii');
right_u = rightpoints(:, 2);
right_v = rightpoints(:,3);

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
        A(i,9) = -object_x(count)*right_u(count);
        A(i,10) = -object_y(count)*right_u(count);
        A(i,11) = -object_z(count)*right_u(count);
        A(i,12) = -right_u(count);

        A(i+1,1) = 0;
        A(i+1,2) = 0;
        A(i+1,3) = 0;
        A(i+1,4) = 0;
        A(i+1,5) = object_x(count);
        A(i+1,6) = object_y(count);
        A(i+1,7) = object_z(count);
        A(i+1,8) = 1;
        A(i+1,9) = -object_x(count)*right_v(count);
        A(i+1,10) = -object_y(count)*right_v(count);
        A(i+1,11) = -object_z(count)*right_v(count);
        A(i+1,12) = -right_v(count);
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
   
    Wr = [f_u 0 u_o; 0 f_v v_o; 0 0 1];
    %disp(Wr);
 
    %calculate rotation and translation
   
   right_tx = (proj(1,4)-(u_o*proj(3,4)))/f_u;
   right_ty = (proj(2,4)-(v_o*proj(3,4)))/f_v;
   right_tz = proj(3,4);
   right_translation = [right_tx; right_ty; right_tz];
   
   right_r1 = [((proj(1,1)-u_o*proj(3,1))/f_u) ((proj(1,2)-u_o*proj(3,2))/f_u) ((proj(1,3)-u_o*proj(3,3))/f_u)];
   right_r2 = [((proj(2,1)-v_o*proj(3,1))/f_v) ((proj(2,2)-v_o*proj(3,2))/f_v) ((proj(2,3)-v_o*proj(3,3))/f_v)];
   right_r3 = [proj(3,1) proj(3,2) proj(3,3)];
   right_rotation = [right_r1; right_r2; right_r3];