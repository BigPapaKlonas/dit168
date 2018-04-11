#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"

#include <cmath>
#include "acceleration.hpp"
#include "yawDegrees.hpp"
#include "distanceChecker.hpp"

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
	//distanceChecker dc;

    	// Instantiate a OD4Session object
    cluon::OD4Session od4(111,[&od4, &x_accel, &y_accel, &z_accel, &speed](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == opendlv::proxy::AccelerationReading::ID()) {
	
            opendlv::proxy::AccelerationReading receivedMsg = cluon::extractMessage<opendlv::proxy::AccelerationReading>(std::move(envelope));
            	x_accel = receivedMsg.accelerationX();
		y_accel = receivedMsg.accelerationY();
		z_accel = receivedMsg.accelerationZ();
		std::cout << "Received x-acceleration: " << receivedMsg.accelerationX() << "." << std::endl;
        }
        else if (envelope.dataType() == opendlv::proxy::GroundSpeedReading::ID()) {
            opendlv::proxy::GroundSpeedReading receivedMsg = cluon::extractMessage<opendlv::proxy::GroundSpeedReading>(std::move(envelope));
            	speed = receivedMsg.groundSpeed();
		std::cout << "Received speed: " << receivedMsg.groundSpeed() << "." << std::endl;
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
		
		steeringAngle = yd.getSteeringAngle(x_accel, y_accel, z_accel);
		
		//Get acceleration(in m/s² using raw accelerometer readings
        	float final_acceleration = a.getAcceleration(x_accel, y_accel);
	 	
        	distanceTraveled = a.getDistanceTraveled(final_acceleration);
	
		//Get speed (in m/s)
		speed = a.getSpeed(final_acceleration, initialVelocity);

		//update initialVelocity
        	initialVelocity = speed; 

		//distanceTraveled = dc.getDistance(speed);
	
		//Sending data through od4 session, using v2v protocol
        	msg.readingDistanceTraveled(distanceTraveled);
		msg.readingSpeed(speed);
		msg.readingSteeringAngle(steeringAngle);

        	od4.send(msg);
   	}
   
    return 0;
}



