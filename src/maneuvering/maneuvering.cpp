// Vera requires this "Copyright" notice
#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <queue>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"

#include "messages.hpp"

#define FOLLOWER 0
#define LEADER 1

using namespace std;
using namespace opendlv::proxy;

int main(int argc, char **argv)
{

    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    uint16_t cidRemote;
    uint16_t cidPwmOds;
    uint16_t cidInternal;
    uint16_t mode = LEADER;

    // In case no CID is provided
    if (commandlineArguments.count("cid_remote") == 0 || commandlineArguments.count("cid_pwm_ods") == 0 ||
            commandlineArguments.count("cid_internal") == 0)
    {
        std::cerr <<"You must specify which OpenDaVINCI session identifiers (CIDs) the "
                "maneuvering script shall listen to!"  << std::endl;
        std::cerr <<"cid_remote for the CID used by the remote_controller microservice\n"
                "cid_pwn_ods for the pwm-motor and odsupercomponent\n"
                "cid_internal for the internal communication CID"<< std::endl;
        std::cerr << "Example: " << argv[0] << " --cid_remote=121 --cid_pwm_ods=120 --cid_internal=122 " << std::endl;
        return -1;
    }
    else
    {
        cidRemote = std::stoi(commandlineArguments["cid_remote"]);
        cidPwmOds = std::stoi(commandlineArguments["cid_pwm_ods"]);
        cidInternal = std::stoi(commandlineArguments["cid_internal"]);
    }

    // Checks CID range
    if ((cidRemote < 1 || cidRemote > 254) || (cidPwmOds < 1 || cidPwmOds > 254))
    {
        std::cerr << "The OpenDaVINCI session identifiers (CIDs) must be in the range 1 to 254"
                  << std::endl;
        return -1;
    }

    // Checks CID range
    if (cidRemote == cidPwmOds)
    {
        std::cerr << "The two OpenDaVINCI session identifiers (CIDs) must not be the same"
                  << std::endl;
        return -1;
    }

    // Initializing od4 session for remote control, odsupercomponent/pwm-motor and internal communication
    cluon::OD4Session od4Remote(cidRemote);
    cluon::OD4Session od4PwmOds(cidPwmOds);
    cluon::OD4Session od4Internal(cidInternal);

    // Check that the od4 sessions are running
    if (od4Remote.isRunning() == 0)
    {
        std::cout << "ERROR: No OD4 responsible for the remote controller is running!" << std::endl;
        return -1;
    } else if (od4PwmOds.isRunning() == 0)
    {
        std::cout << "ERROR: No OD4 responsible for the pwm-motor odsupercomponent is running!" << std::endl;
        return -1;
    }
    else if (od4Internal.isRunning() == 0)
    {
        std::cout << "ERROR: No OD4 responsible for the internal communication is running!" << std::endl;
        return -1;
    }

    // Struct containing the function, delegate, to be called when message, messageIdentifier, is received
    struct onMessageStruct{
        int32_t messageIdentifier;
        std::function< void(cluon::data::Envelope &&envelope)> delegate;
    };
    onMessageStruct messageStruct;

    // Queues containing above structs
    queue<onMessageStruct> queueRemote;
    queue<onMessageStruct> queueInternal;

    GroundSteeringReading msgSteering;
    PedalPositionReading msgPedal;

    // ****** Remote control ****** //

    auto onRemoteModeMessage = [&mode](cluon::data::Envelope &&env){
        RemoteModeMessage msg = cluon::extractMessage<RemoteModeMessage>(std::move(env));
        std::cout << "Setting mode to " << std::endl;
        if (msg.mode())
        {
            std::cout << "LEADER" << std::endl;
        }
        else
        {
            std::cout << "FOLLOWER" << std::endl;
        }
        mode = msg.mode();
    };
    messageStruct.delegate = onRemoteModeMessage;
    messageStruct.messageIdentifier = RemoteModeMessage::ID();
    queueRemote.push(messageStruct);

    auto onPedalPositionReadingMessage = [&mode, &msgPedal, &od4PwmOds](cluon::data::Envelope &&env){
        PedalPositionReading msg = cluon::extractMessage<PedalPositionReading>(std::move(env));
        std::cout << "Received pedal position reading " << msg.percent() << std::endl;
        if(mode == LEADER)
        {
            std::cout << "LEADER MODE ACTIVATED, forwarding pedal reading to pwm/ods " << std::endl;
            msgPedal.percent(msg.percent());
            od4PwmOds.send(msgPedal);
        }
        else
        {
            std::cout << "LEADER DEACTIVATED, ignoring to forward pedal reading" << std::endl;
        }
    };
    messageStruct.delegate = onPedalPositionReadingMessage;
    messageStruct.messageIdentifier = PedalPositionReading::ID();
    queueRemote.push(messageStruct);

    auto onGroundSteeringReadingMessage = [&mode, &msgSteering, &od4PwmOds](cluon::data::Envelope &&env){
        GroundSteeringReading msg = cluon::extractMessage<GroundSteeringReading>(std::move(env));
        std::cout << "Received ground steering reading " << msg.steeringAngle() << std::endl;
        if(mode == LEADER)
        {
            std::cout << "LEADER MODE ACTIVATED, forwarding steering angle to pwm/ods " << std::endl;
            msgSteering.steeringAngle(msg.steeringAngle());
            od4PwmOds.send(msgSteering);
        }
        else
        {
            std::cout << "LEADER DEACTIVATED, ignoring to forward steering angle" << std::endl;
        }
    };
    messageStruct.delegate = onGroundSteeringReadingMessage;
    messageStruct.messageIdentifier = GroundSteeringReading::ID();
    queueRemote.push(messageStruct);

    // Register the lambda functions and message identifiers for each instance in queue
    while (!queueRemote.empty())
    {
        od4Remote.dataTrigger(queueRemote.front().messageIdentifier, queueRemote.front().delegate);
        queueRemote.pop();
    }


    // ****** Internal structs ****** //

    //TODO similar code as in remote controller




    std::cerr << "Maneuvering script v0.1" << std::endl;

    while (od4Remote.isRunning() && od4PwmOds.isRunning() && od4Internal.isRunning())
    {

    }

    return 0;
}