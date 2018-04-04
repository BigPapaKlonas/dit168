#include "yawDegrees.hpp"
#include <math.h>

float yawDegrees::getSteeringAngle(float rad){
	
	float yaw = (rad * 180) / M_PI;
	return yaw;
}

//wget https://github.com/catchorg/Catch2/releases/download/v2.1.1/catch.hpp
