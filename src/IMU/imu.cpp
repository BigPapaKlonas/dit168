//Vera++ requires Copyright notice
#include <iostream>
#include <stdint.h>
#include <chrono>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"
#include <math.h>
#include "acceleration.hpp"
#include "yawDegrees.hpp"

extern "C"
{
#include <rc_usefulincludes.h>
#include <roboticscape.h>
}

int main()
{
 uint8_t distanceTraveled = 0;
 uint8_t steeringAngle = 0;
 float initial_speed = 0.0;
 float yaw = 0.0;
 float sample_rate = 0.01;
 float gyro = 0;
 float old_gyro = 0;
 float z_gyro = 0;
 float old_yaw = 0;
 float distance = 0;
 float speed = 0;

 readingsIMU msg;
 yawDegrees yd;
 acceleration a;
 //This is the container for holding the sensor data from the IMU.
 rc_imu_data_t data;
 // Instantiate a OD4Session object
 cluon::OD4Session od4(111,
    [](cluon::data::Envelope &&envelope) noexcept
     {
      if (envelope.dataType() == 2202)
       {
        readingsIMU ReceivedMsg = cluon::extractMessage
         <readingsIMU>(std::move(envelope));
       }
     });

 //terminate in case no OD4 session running
 if (od4.isRunning() == 0)
  {
    std::cout << "ERROR: No od4 running!!!" << std::endl;
    return -1;
  }

 // initialize hardware first
 if (rc_initialize())
  {
   std::cout << "ERROR: failed to run rc_initialize(), are you root?\n"<< std::endl;
   return -1;
  }

 // use defaults
 rc_imu_config_t conf = rc_default_imu_config();
 rc_is_gyro_calibrated();

 if (rc_initialize_imu(&data, conf))
  {
   std::cout << "rc_initialize_imu_failed\n"<< std::endl;
   return -1;
  }

 while (od4.isRunning())
  {

   //program flow control
   while (rc_get_state() != EXITING)
    {
     if (rc_read_accel_data(&data)<0)
      {
       std::cout <<"read accel data failed\n" << std::endl;
      }
     //XYZ readings of acceleration, in M/s²
     //shifted, since the beaglebone blue is positioned sideways on the smartcar
     float x_accel = data.accel[2];
     float y_accel = data.accel[1];
     //Smoothing the accelerometer readings to remove noice
     if (x_accel<0.2 && x_accel>-0.1){x_accel=0.0;}
     if (y_accel<0.5 && y_accel>-0.1){y_accel=0.0;}
     // print gyro data
     if (rc_read_gyro_data(&data)<0)
      {
       std::cout <<"read gyro data failed\n" << std::endl;
      }

     old_gyro = z_gyro;
     z_gyro = data.gyro[0];
     //Removing noice from gyroscope readings
     if (z_gyro < 1.0 && z_gyro > -1.0){z_gyro = 0.0;}
     //getting the difference in gyroscope readings
     float delta_gyro = old_gyro - z_gyro;
     gyro = gyro + delta_gyro;

     old_yaw = yaw;
     yaw = yd.getHeading(x_accel, y_accel, gyro, sample_rate);
     //Removing the noice from yaw readings
     if (yaw < 1.0 && yaw > -1.0){yaw = 0.0;}
     float delta_yaw = old_yaw - yaw;
     //Broadcast yaw only when there is a significant difference
     if (delta_yaw < -5.0 || delta_yaw > 5.0 || delta_yaw == 0)
      {
       msg.readingSteeringAngle(yaw);
      }

     float accel = a.getAcceleration(x_accel, y_accel);
     initial_speed = speed;
     speed = a.getSpeed(accel, initial_speed);
     //Removing noice from distance readings
     if (a.getDistanceTraveled(accel, sample_rate, speed) > 0.00007)
      {
       distance += a.getDistanceTraveled(accel, sample_rate, speed);
      }
     msg.readingDistanceTraveled(distance);
     od4.send(msg);
    }
  }
  rc_power_off_imu();
  //exit cleanly
  rc_cleanup();
  return 0;
}
