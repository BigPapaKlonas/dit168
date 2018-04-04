#include "acceleration.hpp"
#include <math.h>

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
