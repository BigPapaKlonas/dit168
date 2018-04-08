# Inertial Measurement Unit

Reads the IMU sensor registers. The hardware used is in this project is an 9-Axis IMU with 3 accelerometers, gyroscopes and magnetometers.

Dependent on Robotics Cape library, by [Strawson Design](https://github.com/StrawsonDesign/Robotics_Cape_Installer)


#### Unit test

The unit tests provided makes use of the test framework Catch. 

Included is a generated catch.hpp file. Do not modify this file.
Run tests by running command

` g++ -std=c++11 -o runTest Testacceleration.cpp acceleration.cpp` 

followed by 

` ./runTest`
