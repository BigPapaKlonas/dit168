#include <iostream>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"


extern "C"
{
#include <roboticscape.h>
#include <rc_usefulincludes.h>
}


int main() {

    //local variables
    //to get distance travelled (odometer) = velocity * time
    uint8_t distanceTraveled = 0;
    IMU msg;

    //This is the container for holding the sensor data from the IMU.
    //...
    //float accel[3];	// units of m/s^2
    //float gyro[3]; // units of degrees/s
    //...
    rc_imu_data_t data;

    // Instantiate a OD4Session object
    cluon::OD4Session od4(111,
                          [](cluon::data::Envelope &&envelope) noexcept {
                              if (envelope.dataType() == 2202) {
                                  IMU ReceivedMsg = cluon::extractMessage
                                          <IMU>(std::move(envelope));
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
    //conf::enable_magnetometer = 1;

    if(rc_initialize_imu(&data, conf)){
        std::cout << "rc_initialize_imu_failed\n"<< std::endl;
        return -1;
    }

    while (od4.isRunning()) {

        //program flow control
        //while (rc_get_state() != EXITING) {

        if(rc_read_accel_data(&data)<0){
            td::cout <<"read accel data failed" << std::endl;
        }

        //XYZ readings of acceleration, in M/s²
        float x_accel = data.accel[0];
        float y_accel = data.accel[1];
        //Not useful?
        float z_accel = data.accel[2];

        // print gyro data
        if(rc_read_gyro_data(&data)<0){
            td::cout <<"read gyro data failed\n" << std::endl;
        }
        float x_gyro = data.gyro[0];
        float y_gyro = data.gyro[1];
        //not useful?
        float z_gyro = data.gyro[2];

        //*****Start of some great math businezz

        //issue: how to find out time passed? Mock for now.
        float time = 10.0;

        float initialVelocity = 0.0;
        float deltaVelocity = (x_accel * time);
        float currentVelocity = (deltaVelocity - initialVelocity);

        distanceTraveled = (currentVelocity * time);
        //******End of some great amazing math bsnz

        msg.IMUReading(distanceTraveled);
        od4.send(msg);

        //update initialVelocity
        initialVelocity = currentVelocity;

        rc_usleep(100000);
       // }


    }


    //rc_power_off_imu();
    // exit cleanly
    rc_cleanup();
    return 0;
}





