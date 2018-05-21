//Vera++ requires Copyright notice
/*
* This file has been modified to exclude Libcluon, in order to function on the demo 23rd of May. 
*/
#include "imu.hpp"
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
 //auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
 //uint8_t cidInternal;
 
 readingsIMU msg;
 yawDegrees yd;
 acceleration a;
 //This is the container for holding the sensor data from the IMU.
 rc_imu_data_t data;

/* if (commandlineArguments.count("cid_internal") == 0)
  {
   std::cerr <<"You must provide command line arguments for CIDs" << std::endl;
   return -1;
  }
 else
  {
   cidInternal = stoi(commandlineArguments["cid_internal"]);
  }
 // Checks data provided from command line arguments
 if (cidInternal < 120 || cidInternal > 129)
  {
   std::cerr << "The OpenDaVINCI session identifiers (CIDs) must be in the range 120 to 129"
   << std::endl;
   return -1;
  }

 cluon::OD4Session od4(cidInternal);

 //terminate in case no OD4 session running
 if (od4.isRunning() == 0)
  {
    std::cout << "ERROR: No od4 running!!!" << std::endl;
    return -1;
  }
*/
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

// while (od4.isRunning())
//  {

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
       //msg.readingSteeringAngle(yaw);
       //Print line has been added to demo
       printf("yaw: %4.2f degrees\n", yaw);
      }

     float accel = a.getAcceleration(x_accel, y_accel);
     initial_speed = speed;
     speed = a.getSpeed(accel, initial_speed);
     //msg.readingSpeed(speed);
     //Removing noice from distance readings
     if (a.getDistanceTraveled(accel, sample_rate, speed) > 0.00007)
      {
       distance += a.getDistanceTraveled(accel, sample_rate, speed);
      }
     //msg.readingDistanceTraveled(distance);
     //od4.send(msg);
    }
  //}
  rc_power_off_imu();
  //exit cleanly
  rc_cleanup();
  return 0;
}
