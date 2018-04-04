#ifndef ACCELERATION
#define ACCELERATION
#include <cstdint>

class acceleration {
public :
	float getDistanceTraveled(float accel);
	float getSpeed(float accel, float initialVelocity);
};
#endif
