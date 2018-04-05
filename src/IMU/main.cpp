#include <iostream>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"
#include <cmath> //wtf is this one?
#include <math.h>
#include "roboticscape.h"
#include <rc_usefulincludes.h>
#include "acceleration.hpp"
#include "yawDegrees.hpp"

int main() {

	//to get distance travelled (odometer) = velocity * time
    	uint8_t distanceTraveled = 0;
	uint8_t speed = 0;
	uint8_t steeringAngle = 0;
	float initialVelocity = 0.0;
	float yaw = 0.0;
 
    	readingsIMU msg;
	
    	yawDegrees yd;
    	acceleration a;	

    	//This is the container for holding the sensor data from the IMU.
    	//float accel[3];	// units of m/s^2
    	//float gyro[3]; // units of degrees/s
    	rc_imu_data_t data;

    	// Instantiate a OD4Session object
    	cluon::OD4Session od4(111,
                          [](cluon::data::Envelope &&envelope) noexcept {
                              if (envelope.dataType() == 2202) {
                                  readingsIMU ReceivedMsg = cluon::extractMessage
                                          <readingsIMU>(std::move(envelope));
                              }
                          });

    	//terminate in case no OD4 session running
    	if(od4.isRunning() == 0)
    	{
        	std::cout << "ERROR: No od4 running!!!" << std::endl;
        	return -1;
    	}

    	// initialize hardware first
    	if(rc_initialize()){
        	std::cout << "ERROR: failed to run rc_initialize(), are you root?\n"<< std::endl;
        	return -1;
    	}

    	// use defaults
     	rc_imu_config_t conf = rc_default_imu_config();
    	conf::enable_magnetometer = 1;

    	if(rc_initialize_imu(&data, conf)){
        	std::cout << "rc_initialize_imu_failed\n"<< std::endl;
        	return -1;
    	}
	
	//-----DMP settings------
	//Setting up dmp mode, in order to get raw compass data from magnetometer
	//OBS: might be conflict with above initialization, test! (reason: random and DMP cannot be active at same time?)
	if(rc_initialize_imu_dmp(&data, conf)){
		std::cout << "rc_initialize_imu_dmp_failed\n"<< std::endl;
        	return -1;
	}
	conf.dmp_sample_rate = 100;

    	
	while (od4.isRunning()) {

        //program flow control
        //while (rc_get_state() != EXITING) {

        if(rc_read_accel_data(&data)<0){
            td::cout <<"read accel data failed" << std::endl;
        }

        //XYZ readings of acceleration, in M/s²
        float x_accel = data.accel[0];
        float y_accel = data.accel[1];
        float z_accel = data.accel[2];
	

        // print gyro data
         if(rc_read_gyro_data(&data)<0){
             std::cout <<"read gyro data failed\n" << std::endl;
         }

	// read magnetometer data, return uT. Might not be of use.
         if(rc_read_mag_data(&data)<0){
             std::cout <<"read gyro data failed\n" << std::endl;
         }

	//------Three different ways to get yaw, pick one--------	
	
	// 1. getting raw heading from magnetometer, in radians
	yaw = data.compass_heading_raw;
	
	// 2. Compass heading from filtered accel and gyro	
	yaw = data.compass_heading;

	// 3. radians pitch roll and yaw
	yaw = data.fused_TaitBryan[2]; 

	//------End of three different ways to get yaw--------    

	//Get acceleration(in m/s² using raw accelerometer readings
        
	float acceleration = a.getAcceleration(x_accel, y_accel);
	 
	//Get distance traveled (in m)
        distanceTraveled = a.getDistanceTraveled(acceleration);

	//Get speed (in m/s)
	speed = a.getSpeed(acceleration, initialVelocity); 
	
	//calculate yaw (or heading(or steeringAngle)) ---No longer necessary?
	//steeringAngle = yd.getSteeringAngle(x_accel, y_accel, z_accel);	

        msg.readingDistanceTraveled(distanceTraveled);
	msg.readingSpeed(currentVelocity);
	msg.readingSteeringAngle(steeringAngle);

        od4.send(msg);

        //update initialVelocity
        initialVelocity = speed;

        //rc_usleep(100000);
        // }

    }


    //rc_power_off_imu();
    // exit cleanly
    rc_cleanup();
    return 0;
}





