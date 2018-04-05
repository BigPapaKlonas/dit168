#include "yawDegrees.hpp"
#include <math.h>
#include <iostream>
#include <cmath>

//method used to determine yaw (rotation in the z-axis, i.e steering angle), using raw 
//acceleration reedings in 3D room
double yawDegrees::getSteeringAngle(float x_accel, float y_accel, float z_accel){
	
	double y = std::abs(y_accel);
    	double x = std::abs(x_accel);

    	double yawRad = acos(y/(pow(y,2) + pow(x,2)));

    	double yaw = (yawRad * 180) / M_PI;

    	yaw = floor(yaw * 100.) / 100;

    	if(y_accel < 0){ yaw = yaw * -1;}

	return yaw;
}

//wget https://github.com/catchorg/Catch2/releases/download/v2.1.1/catch.hpp
