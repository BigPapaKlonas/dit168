//Vera requires Copyright claim
#include "yawDegrees.hpp"
#include <math.h>
#include <iostream>
#include <cmath>

//method used to determine yaw (rotation in the z-axis, i.e steering angle),
//using raw acceleration reedings in 3D room
double yawDegrees::getSteeringAngle(float x_accel, float y_accel)
 {
  double yaw;
  double yawRad;
  if (y_accel < 0)
   {
    double y = std::abs(y_accel);
    double x = std::abs(x_accel);

    yawRad = acos(y/(sqrt(pow(y, 2) + pow(x, 2))));
    yaw =  (yawRad * 180) / M_PI;
    yaw = floor(yaw * 100) / 100;
    yaw = yaw * -1;
    return yaw;
   }
  else
   {
    yawRad = atan2(x_accel, y_accel);
    yaw =  (yawRad * 180) / M_PI;
    yaw = floor(yaw * 100) / 100;
    return yaw;
   }
 }
//get theta using complementary filter combining readings
float yawDegrees::getHeading(float x_accel, float y_accel, float z_gyro, float deltaTime)
 {
  float lpf = 0.02;
  float hpf = 0.98;
  float accAngle = atan2(x_accel, y_accel);
  float radTheta = hpf * (z_gyro * deltaTime) + lpf * accAngle;
  float theta = (radTheta * 180) / M_PI;
  theta = floor(theta * 100) /100;
  return theta;
 }

float yawDegrees::getGyro(float old_gyro, float z_gyro)
 {
  float gyro;
  if (z_gyro < 1.0 && z_gyro > -1.0){z_gyro = 0.0;}
  float delta_gyro = old_gyro - z_gyro;
  gyro = gyro + delta_gyro;
  return gyro;
 }
