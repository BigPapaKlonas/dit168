#ifndef ACCELERATION
#define ACCELERATION
#include <cstdint>

class acceleration {
public :
	float getAcceleration(float x_accel, float y_accel);
	float getDistanceTraveled(float accel);
	float getSpeed(float accel, float initialVelocity);
};
#endif
