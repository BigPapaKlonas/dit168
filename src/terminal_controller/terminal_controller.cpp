// Vera requires this "Copyright" notice - dashFtabs 2018
#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"

int main(int argc, char **argv)
{

    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    int CID;

    // In case no CID is provided
    if (commandlineArguments.count("cid") == 0)
    {
        std::cerr << "You must specify which OpenDaVINCI session identifier (CID) the "
                "terminal_controller "
                "shall use (most likely the same as the pwm-motor and the odsupercomponent are "
                "using)." << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=120" << std::endl;
        return -1;
    }
    else
    {
        CID = std::stoi(commandlineArguments["cid"]);
    }

    // Checks CID range
    if (CID <= 120 || CID >= 129)
    {
        std::cerr << "The OpenDaVINCI session identifier (CID) must be in the range 1 to 254"
                  << std::endl;
        return -1;
    }

    // Message objects used to broadcast into specified OD4 session
    opendlv::proxy::GroundSteeringReading msgSteering;
    opendlv::proxy::PedalPositionReading msgPedal;

    // Variables
    float speed = 0.3;
    float steeringAngle = 15.0;
    float stopBeforeObstacleDistance = 0.45;
    bool moving = false;
    bool exit = false;
    char command;
    char const controls[] =
            "\nCommands: \n"
            "w - move forward \n"
            "a - turn left \n"
            "d - turn right \n"
            "# (other key) - stop \n\n"

            "v - set speed \n"
            "b - set angle \n"
            "n - set distance for obstacle breaking\n\n"
            "e - exit \n";

    // Initializing od4 session
    cluon::OD4Session od4(CID, [&msgPedal, &od4, &stopBeforeObstacleDistance, &moving]
            (cluon::data::Envelope &&envelope) noexcept
    {
        if (envelope.dataType() == opendlv::proxy::DistanceReading::ID())
        {
            opendlv::proxy::DistanceReading receivedMsg =
                    cluon::extractMessage<opendlv::proxy::DistanceReading>(
                    std::move(envelope));

            // Stops the car in case obstacle is detected within specified distance
            if (moving && (receivedMsg.  distance() < stopBeforeObstacleDistance))
            {
                moving = false;
                msgPedal.percent(0.0);
                od4.send(msgPedal);
                std::cout << "STOPPING! Obstacle detected in" << receivedMsg.distance() << " m!"
                          << std::endl;
            }
        }
    });

    std::cerr << "Group1's remote terminal control v1.0" << std::endl;

    if (od4.isRunning() == 0)
    {
        std::cout << "ERROR: No OD4 is running!" << std::endl;
        return -1;
    }

    std::cout << controls << std::endl;

    while (od4.isRunning() && exit == false)
    {
        std::cout << "To see the controls press c" << std::endl;

        std::cin >> command;

        switch (command)
        {

            case 'w':
                std::cout << "Forward" << std::endl;
                msgSteering.steeringAngle(0.0);
                od4.send(msgSteering);
                msgPedal.percent(speed);
                od4.send(msgPedal);
                moving = true;
                break;
            case 'a':
                std::cout << "Left" << std::endl;
                msgSteering.steeringAngle(steeringAngle);
                od4.send(msgSteering);
                msgPedal.percent(speed);
                od4.send(msgPedal);
                moving = true;
                break;
            case 'd':
                std::cout << "Right" << std::endl;
                msgSteering.steeringAngle(-steeringAngle);
                od4.send(msgSteering);
                msgPedal.percent(speed);
                od4.send(msgPedal);
                moving = true;
                break;
            case 'e':
                msgPedal.percent(0);
                od4.send(msgPedal);
                moving = false;
                std::cout << "Exits.. " << std::endl;
                exit = true;
                break;
            case 'v':
                msgPedal.percent(0);
                od4.send(msgPedal);
                moving = false;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Speed - enter a value in the range 0.15 to 1.00: " << std::endl;
                std::cin >> speed;
                while (speed < 0.15 || speed > 1.00)
                {
                    std::cout << "ERROR: the value must be in the range 0.15 to 1.00! Try again: "
                              << std::endl;
                    std::cin >> speed;
                }
                std::cout << "Speed successfully set to: " << speed << std::endl;
                break;
            case 'b':
                msgPedal.percent(0);
                od4.send(msgPedal);
                moving = false;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Angle - enter a value in the range 0 to 38: " << std::endl;
                std::cin >> steeringAngle;
                while (steeringAngle < 0 || steeringAngle > 38)
                {
                    std::cout << "ERROR: the value must be in the range 0 to 38! Try again: "
                              << std::endl;
                    std::cin >> steeringAngle;
                }
                std::cout << "Steering angle successfully set to: " << steeringAngle << std::endl;
                break;
            case 'n':
                msgPedal.percent(0);
                od4.send(msgPedal);
                moving = false;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Distance to obstacle when start breaking - enter a value in the range"
                        "0.0 - 1.0: " << std::endl;
                std::cin >> stopBeforeObstacleDistance;
                while (stopBeforeObstacleDistance < 0 || stopBeforeObstacleDistance > 1)
                {
                    std::cout << "ERROR: the value must be in the range 0.0 to 1.0! Try again: "
                              << std::endl;
                    std::cin >> stopBeforeObstacleDistance;
                }
                std::cout << "Break distance to obstacle successfully set to: "
                          << stopBeforeObstacleDistance << std::endl;
                break;
            case 'c':
                msgSteering.steeringAngle(0.0);
                od4.send(msgSteering);
                msgPedal.percent(0);
                od4.send(msgPedal);
                moving = false;
                std::cout << controls << std::endl;
                break;
            default:
                std::cout << "Stop!" << std::endl;
                msgSteering.steeringAngle(0.0);
                od4.send(msgSteering);
                msgPedal.percent(0);
                od4.send(msgPedal);
                moving = false;
                break;
        }
    }

    return 0;
}
