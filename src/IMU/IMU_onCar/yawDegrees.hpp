#ifndef YAWDEGREES
#define YAWDEGREES
#include <cstdint>

class yawDegrees {
public :
	double getSteeringAngle(float x_accel, float y_accel);
	float getHeading(float x_accel, float y_accel, float z_gyro, float deltaTime);
};
#endif
