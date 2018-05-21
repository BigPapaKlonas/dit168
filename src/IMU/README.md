### Note 2018-05-21

This version has been modified to exclude Libcluon, in order to work for the demo on the 23rd of May.

# Inertial Measurement Unit

Reads the IMU sensor registers. The hardware used is in this project is a 9-Axis IMU with 3 accelerometers, gyroscopes and magnetometers.

Dependent on Robotics Cape library, by [Strawson Design](https://github.com/StrawsonDesign/Robotics_Cape_Installer)


### Unit test

#### Compiling unit tests manually

The unit tests provided makes use of the test framework Catch. 

Included is a generated catch.hpp file. Do not modify this file.
Run tests by running command

` g++ -std=c++11 -o runTest Testacceleration.cpp acceleration.cpp ` 

followed by 

` ./runTest `

#### Compiling unit tests using cmake

Run the following commands in order to complie unit tests using CMakeLists:

` mkdir build && cd build `

` cmake .. ` 

` make && make test`

### Run IMU service on car
Take these steps in order to run the IMU service on the smart car: 
1. .zip the IMU_onCar folder 

2. Connect to the car. Secure copy the .zip file onto the car by running 

` scp IMU.zip debian@192.168.8.1:~/` and input password

3. ssh onto the car by running 

` ssh debian@192.168.8.1 ` and input password

4. unzip the IMU_onCar folder to a directory 

`unzip IMU_onCar.zip -d IMU `

5. In the directory, run

` mkdir build && cd build && cmake .. && make && sudo ./imu` and input password
