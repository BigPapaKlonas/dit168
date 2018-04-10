#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"

#include <cmath>
#include "acceleration.hpp"
#include "acceleration.cpp"
#include "yawDegrees.hpp"

int main() {

    	uint8_t distanceTraveled = 0;
	uint8_t speed = 0;
	uint8_t steeringAngle = 0;
	float initialVelocity = 0.0;

	//XYZ readings of acceleration, in M/s²
        float x_accel = 0.0;
        float y_accel = 0.0;
        float z_accel = 0.0;
 
    	readingsIMU msg;

    	yawDegrees yd;
    	acceleration a;

    	// Instantiate a OD4Session object
    	cluon::OD4Session od4(111,
                          [&od4, &x_accel, &y_accel, &z_accel](cluon::data::Envelope &&envelope) noexcept {
              			if(envelope.dataType() == opendlv::proxy::AccelerationReading::ID()) {
            				opendlv::proxy::AccelerationReading receivedMsg = cluon::extractMessage<opendlv::proxy::AccelerationReading>(
						std::move(envelope));

						x_accel = receivedMsg.accelerationX();
						y_accel = receivedMsg.accelerationY();
						z_accel = receivedMsg.accelerationZ();
						std::cout << "Received x-acceleration: " << receivedMsg.accelerationX() << "." << std::endl;
				}
                          });

    	//terminate in case no OD4 session running
    	if(od4.isRunning() == 0)
    	{
        	std::cout << "ERROR: No od4 running!!!" << std::endl;
        	return -1;
    	}

    	//using namespace std::literals::chrono_literals;
	while (od4.isRunning()) {
		// + 1. Manually. calculate yaw (or heading(or steeringAngle)) from accelerometer readings
		steeringAngle = yd.getSteeringAngle(x_accel, y_accel, z_accel);

		//Get acceleration(in m/s² using raw accelerometer readings
        	float acceleration = a.getAcceleration(x_accel, y_accel);
	 
		//Get distance traveled (in m)
        	distanceTraveled = a.getDistanceTraveled(acceleration);
	
		//Get speed (in m/s)
		speed = a.getSpeed(acceleration, initialVelocity);

		//update initialVelocity
        	initialVelocity = speed; 
	
		//Sending data through od4 session, using v2v protocol
        	msg.readingDistanceTraveled(distanceTraveled);
		msg.readingSpeed(speed);
		msg.readingSteeringAngle(steeringAngle);

        	od4.send(msg);
   	}
   
    return 0;
}





