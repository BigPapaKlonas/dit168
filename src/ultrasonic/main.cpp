#include <iostream>
#include <chrono>
#include <thread>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"

// Includes Robotics Cape libraries written in C
extern "C"
{
#include <rc_usefulincludes.h>
#include <roboticscape.h>
}

// Global definitions
#define BUS                             1
#define DEV_ADDRESS_ULTRASONIC_FRONT    0x71
// Read
#define SOFTWARE_REVISION               0x00
#define FIRST_ECHO_LOW_BYTE_REGISTER    0x03
// Write
#define COMMAND_REGISTER                0x00
// Units
#define CENTIMETER                      0x51

int main() {

    // Local variables
    uint8_t reading_byte = 0;
    uint8_t version_byte = 0;
    UltrasonicFront msg;

    // Instantiate a OD4Session object
    cluon::OD4Session od4(111,
                          [](cluon::data::Envelope &&envelope) noexcept
                          {
                              //TODO: remove this after debugging is done
                              // Checks if received data is ultrasonic reading
                              if (envelope.dataType() == 2201)
                              {
                                  // Extract message
                                  UltrasonicFront receivedMsg=cluon::extractMessage
                                          <UltrasonicFront>(std::move(envelope));
                                  // Printing the received message
                                  printf("ultra reading: %u cm\n", receivedMsg.readingCm());
                              }
                          });

    // Checks if a OD4 session is running, else; terminates
    if (!od4.isRunning()) {
        std::cout << "ERROR: no OD4 session running\n" << std::endl;
        return -1;
    }

    // Initialize cape library
    std::cout << "Initialize roboticscape library" << std::endl;
    if (rc_initialize()) {
        std::cout << "ERROR: failed to initialize rc_initialize(), are you root?\n" << std::endl;
        return -1;
    }

    // Initialize bus where ultrasonics are located
    std::cout << "Initialize bus" << std::endl;
    if(rc_i2c_init(BUS, DEV_ADDRESS_ULTRASONIC_FRONT)){
        std::cout << "ERROR: failed to initialize bus\n" << std::endl;
        return -1;
    }


    using namespace std::literals::chrono_literals;
    while (od4.isRunning()) {

        //rc_i2c_claim_bus(BUS);

        // Triggers ultrasonic sensor to store a centimeter reading in its register
        rc_i2c_write_byte(BUS, COMMAND_REGISTER, CENTIMETER);

        // Preventing deadlocks
        std::this_thread::sleep_for(0.005s);

        // Loops while the sensor is ranging
        rc_i2c_read_byte(BUS, SOFTWARE_REVISION, &version_byte);
        while (version_byte != 11) {
            rc_i2c_read_byte(BUS, SOFTWARE_REVISION, &version_byte);
        }

        //TODO: remove this after debugging is done
        rc_i2c_read_byte(BUS, FIRST_ECHO_LOW_BYTE_REGISTER, &reading_byte);
        printf("Reading: %u cm\n", reading_byte);

        msg.readingCm(reading_byte);
        // Sends message to OD4 session
        od4.send(msg);

        // Preventing deadlocks
        std::this_thread::sleep_for(0.01s);
    }

    // exit cleanly
    //roboticscape::rc_i2c_release_bus(BUS);
    rc_cleanup();
    return 0;
}