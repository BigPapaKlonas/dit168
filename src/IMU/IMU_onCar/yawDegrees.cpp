#include "yawDegrees.hpp"
#include <math.h>
#include <iostream>
#include <cmath>

//method used to determine yaw (rotation in the z-axis, i.e steering angle), using raw 
//acceleration reedings in 3D room
double yawDegrees::getSteeringAngle(float x_accel, float y_accel){
	
	double y = std::abs(y_accel);
    	double x = std::abs(x_accel);

    	double yawRad = acos(y/(pow(y,2) + pow(x,2)));

    	double yaw = (yawRad * 180) / M_PI;

    	yaw = floor(yaw * 100) / 100;

    	if(y_accel < 0) yaw = yaw * -1;
	//testing shit
	double a = sqrt(pow(x_accel,2)+pow(x_accel,2)+pow(9.82,2)- 9.82);
	return a;
}

float yawDegrees::getHeading(float x_accel, float y_accel, float z_gyro, float heading, float deltaTime){
	float lowPassFilter = 0.02;
	float highPassFilter = 0.98;
	
	//both works to get accel angle
	float accAngle = atan2(x_accel, y_accel);
	float radTheta = highPassFilter * (heading + z_gyro * deltaTime) + lowPassFilter * accAngle;
	float theta = (radTheta * 180) / M_PI;
	return theta;

}

//dont forget to account for negative numbers!


