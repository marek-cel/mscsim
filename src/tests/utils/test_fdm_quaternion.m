pkg load quaternion

q1 = quaternion( 1.0, 2.0, 3.0, 4.0 );
disp(q1.w),disp(q1.x),disp(q1.y),disp(q1.z);

q2 = conj(q1);
disp(q2.w),disp(q2.x),disp(q2.y),disp(q2.z);

q3 = quaternion( -0.707107, 0.408248, 0.408248, 0.408248 );

q4 = inv(q3);
disp(q4.w),disp(q4.x),disp(q4.y),disp(q4.z);

omega = [ 1.0, 2.0, 3.0 ];
q5 = diff(q4, -omega);
disp(q5.w),disp(q5.x),disp(q5.y),disp(q5.z);