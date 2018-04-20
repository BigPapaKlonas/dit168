#include "acceleration.hpp"
#include <math.h>

float acceleration::getAcceleration(float x_accel, float y_accel){
	
	//getting acceleration, derived from x-acceleration and y-acceleration raw readings and pythagorean theorem
	float hypo_accel = pow(x_accel,2) + pow(y_accel,2);
    	hypo_accel = sqrt(hypo_accel);
	//rounding decimals
	hypo_accel = floor(hypo_accel * 100) / 100;
	return hypo_accel;	
}


float acceleration::getDistanceTraveled(float accel){
	
	//issue: how to find out time passed? Mock for now.
	//Edit: sample rate in Hz? 
        float time = 0.01;
        
	float distanceTraveled = (accel * pow(time,2)) / 2;
	distanceTraveled = floor(distanceTraveled * 100000) / 100000;
	//distanceTraveled = ceilf(distanceTraveled* 100000) / 100000;
	return distanceTraveled;	
}

float acceleration::getSpeed(float accel, float initialVelocity){
	//issue: how to find out time passed? Mock for now.
        //Edit: sample rate in Hz? 
        float time = 0.01;
        
	float deltaVelocity = (accel * time);
        float speed = (deltaVelocity - initialVelocity);
	return speed;	
}
