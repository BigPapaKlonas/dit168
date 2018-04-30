//Vera++ requires Copyright notice
#include "acceleration.hpp"
#include <math.h>

float acceleration::getAcceleration(float x_accel, float y_accel){
	float hypo_accel = pow(x_accel, 2) + pow(y_accel, 2);
    	hypo_accel = sqrt(hypo_accel);
	return hypo_accel;
}

float acceleration::getDistanceTraveled(float accel, float time, float speed){
	float distanceTraveled = (speed * time)+((accel * pow(time, 2)) / 2);
	return distanceTraveled;
}

float acceleration::getSpeed(float accel, float initialVelocity){
        float time = 0.01;
	float deltaVelocity = (accel * time);
        float speed = (deltaVelocity - initialVelocity);
	speed = std::abs(speed);
	return speed;
}
