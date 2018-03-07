#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"

#include "messages.hpp"

int main(int /*argc*/, char ** /*argv*/) {

    cluon::OD4Session od4(111, [](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == opendlv::proxy::GroundSteeringReading::ID()) {
            opendlv::proxy::GroundSteeringReading receivedMsg = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(
                    std::move(envelope));
            std::cout << "Request for steering angle: " << receivedMsg.steeringAngle() << " sent." << std::endl;
        } else if (envelope.dataType() == opendlv::proxy::PedalPositionReading::ID()) {
            opendlv::proxy::PedalPositionReading receivedMsg = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(
                    std::move(envelope));
            std::cout << "Request for pedal position: " << receivedMsg.percent() << " sent." << std::endl;
        }
    });

    if (od4.isRunning() == 0) {
        std::cout << "ERROR: No OD4 is running!" << std::endl;
        return -1;
    } else {
        std::cout << "OD4 is running and you're good to go!\n" << std::endl;
    }

    std::cout << "Commands: \n"
            "w - move forward \n"
            "a - turn left \n"
            "d - turn right \n"
            "any other key - stop \n\n"
            "v - set speed \n"
            "b - set angle \n\n"
            "e - exit \n"
              << std::endl;

    opendlv::proxy::GroundSteeringReading msgSteering;
    opendlv::proxy::PedalPositionReading msgPedal;

    float speed = 0.3;
    float steeringAngle = 15.0;
    bool exit = false;
    char command;

    while (od4.isRunning() && !exit) {
        std::cin >> command;

        switch (command) {

            case 'w':
                std::cout << "Forward" << std::endl;
                msgSteering.steeringAngle(0.0);
                od4.send(msgSteering);
                msgPedal.percent(speed);
                od4.send(msgPedal);
                break;
            case 'a':
                std::cout << "Left" << std::endl;
                msgSteering.steeringAngle(steeringAngle);
                od4.send(msgSteering);
                msgPedal.percent(speed);
                od4.send(msgPedal);
                break;
            case 'd':
                std::cout << "Right" << std::endl;
                msgSteering.steeringAngle(-steeringAngle);
                od4.send(msgSteering);
                msgPedal.percent(speed);
                od4.send(msgPedal);
                break;
            case 'e':
                std::cout << "Exits.. " << std::endl;
                exit = true;
                break;
            case 'v':
                msgPedal.percent(0);
                od4.send(msgPedal);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                std::cout << "Speed - enter a value in the range 0.15 to 1.00: " << std::endl;
                std::cin >> speed;
                while (speed < 0.15 || speed > 1.00) {
                    std::cout << "ERROR: the value must be in the range 0.15 to 1.00! Try again: " << std::endl;
                    std::cin >> speed;
                }
                std::cout << "Speed successfully set to: " << speed << std::endl;
                break;
            case 'b':
                msgPedal.percent(0);
                od4.send(msgPedal);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Angle - enter a value in the range 0 to 20: " << std::endl;
                std::cin >> steeringAngle;
                while (steeringAngle < 0 || steeringAngle > 20) {
                    std::cout << "ERROR: the value must be in the range 0 to 20! Try again: " << std::endl;
                    std::cin >> steeringAngle;
                }
                std::cout << "Steering angle successfully set to: " << steeringAngle << std::endl;
                break;
            default:
                std::cout << "Stop!" << std::endl;
                msgSteering.steeringAngle(0.0);
                od4.send(msgSteering);
                msgPedal.percent(0);
                od4.send(msgPedal);
                break;
        }
    }

    return 0;
}

