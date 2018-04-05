#include "acceleration.hpp"
#include <math.h>

float acceleration::getAcceleration(float x_accel, float y_accel){
	
	//getting acceleration, derived from x-acceleration and y-acceleration raw readings and pythagorean theorem
	float acceleration = pow(x_accel,2) + pow(y_accel,2);
    	acceleration = sqrt(acceleration);
	acceleration = floor(acceleration * 100.) / 100;
	return acceleration;
}


float acceleration::getDistanceTraveled(float accel){
	
	//issue: how to find out time passed? Mock for now.
        float time = 4.0;
        
	float distanceTraveled = accel * pow(time,2);
	return distanceTraveled;	
}

float acceleration::getSpeed(float accel, float initialVelocity){
	//issue: how to find out time passed? Mock for now.
        float time = 4.0;
        
	float deltaVelocity = (accel * time);
        float speed = (deltaVelocity - initialVelocity);
	return speed;	
}
