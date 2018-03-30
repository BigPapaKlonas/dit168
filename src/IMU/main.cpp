#include <iostream>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"


extern "C"
{
#include <roboticscape.h>
}


int main() {

    cluon::OD4Session od4(111,
	[](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == 2202) {
            //TODO: stuff
        }
    });

    if(od4.isRunning() == 0)
    {
        std::cout << "ERROR: No od4 running!!!" << std::endl;
        return -1;
    }


    //https://github.com/StrawsonDesign/Robotics_Cape_Installer/blob/master/examples/rc_test_imu/rc_test_imu.c
    // initialize hardware first
    if(rc_initialize()){
	    std::cout << "ERROR: failed to initialize rc_initialize(), are you root?\n" << std::endl;
		return -1;
	}

    // use defaults for now, except also enable magnetometer.
    rc_imu_config_t conf = rc_default_imu_config();
    conf::enable_magnetometer = 1;

    if(rc_initialize_imu(&data, conf)){
        std::cout <<"rc_initialize_imu_failed \n" << std::endl;
        return -1;
    }


}









