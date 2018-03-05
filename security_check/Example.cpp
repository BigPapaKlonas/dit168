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
            std::cout << "Sent a message for ground steering to " << receivedMsg.steeringAngle() << "." << std::endl;
        } else if (envelope.dataType() == opendlv::proxy::PedalPositionReading::ID()) {
            opendlv::proxy::PedalPositionReading receivedMsg = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(
                    std::move(envelope));
            std::cout << "Sent a message for pedal position to " << receivedMsg.percent() << "." << std::endl;
        }
    });

    if (od4.isRunning() == 0) {
        std::cout << "ERROR: No od4 running!!!" << std::endl;
        return -1;
    } else {
        std::cout << "OD4 is running and you're good to go!" << std::endl;
    }

    std::cout << "Commands: \n"
            "w - move forward \n"
            "s - move backwards \n"
            "d - turn right \n"
            "a - turn left \n"
            "any other key - stop \n" << std::endl;

    opendlv::proxy::GroundSteeringReading msgSteering;
    opendlv::proxy::PedalPositionReading msgPedal;

    double speed = 0.4;
    double steeringAngle = 15.0;

    char command;

    while (od4.isRunning()){
        std::cin >> command;

        switch (command) {

            case 'w':
                std::cout << "Forwards" << std::endl;
                msgSteering.steeringAngle(0.0);
                od4.send(msgSteering);
                msgPedal.percent(speed);
                od4.send(msgPedal);
                break;
            case 'a':
                std::cout << "Left" << std::endl;
                msgSteering.steeringAngle(-steeringAngle);
                od4.send(msgSteering);
                msgPedal.percent(speed);
                od4.send(msgPedal);
                break;
            case 'd':
                std::cout << "Right" << std::endl;
                msgSteering.steeringAngle(steeringAngle);
                od4.send(msgSteering);
                msgPedal.percent(speed);
                od4.send(msgPedal);
                break;
            case 's':
                std::cout << "Backwards" << std::endl;
                msgSteering.steeringAngle(0.0);
                od4.send(msgSteering);
                msgPedal.percent(-speed);
                od4.send(msgPedal);
                break;
            default:
                std::cout << "stop!" << std::endl;
                msgSteering.steeringAngle(0.0);
                od4.send(msgSteering);
                msgPedal.percent(0);
                od4.send(msgPedal);
                break;
        }
    }

    return 0;
}

