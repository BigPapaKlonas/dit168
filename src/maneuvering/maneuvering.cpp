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
    uint16_t cidPwmOds;
    uint16_t cidInternal;
    uint16_t mode = LEADER;
    bool emergencyBreak = false;

    // In case no CID is provided
    if (commandlineArguments.count("cid_pwm_ods") == 0 || commandlineArguments.count("cid_internal") == 0)
    {
        cerr <<"You must specify which OpenDaVINCI session identifiers (CIDs) the "
                    "maneuvering script shall listen to!"  << std::endl;
        cerr <<"cid_pwn_ods for the pwm-motor and odsupercomponent microservice\n"
                    "and cid_internal for the internal communication CID"<< std::endl;
        cerr << "Example: " << argv[0] << " --cid_pwm_ods=120 --cid_internal=122 " << endl;
        return -1;
    }
    else
    {
        cidPwmOds = stoi(commandlineArguments["cid_pwm_ods"]);
        cidInternal = stoi(commandlineArguments["cid_internal"]);
    }

    if (cidInternal < 120 || cidInternal > 129 || cidPwmOds < 120 || cidPwmOds > 129)
    {
        cerr << "The OpenDaVINCI session identifiers (CIDs) must be in the range 120 to 129"
                  << endl;
        return -1;
    }

    // Checks CID range
    if (cidInternal == cidPwmOds)
    {
        cerr << "The two OpenDaVINCI session identifiers (CIDs) must not be the same"
                  << endl;
        return -1;
    }

    // Initializing od4 session for odsupercomponent/pwm-motor and internal communication
    cluon::OD4Session od4PwmOds(cidPwmOds);
    cluon::OD4Session od4Internal(cidInternal);

    // Check that the od4 sessions are running
     if (od4PwmOds.isRunning() == 0)
    {
        cout << "ERROR: No OD4 responsible for the pwm-motor odsupercomponent is running!" << endl;
        return -1;
    }
    else if (od4Internal.isRunning() == 0)
    {
        cout << "ERROR: No OD4 responsible for the internal communication is running!" << endl;
        return -1;
    }

    // Struct containing the function, delegate, to be called when message, messageIdentifier, is received
    struct onMessageStruct{
        int32_t messageIdentifier;
        function< void(cluon::data::Envelope &&envelope)> delegate;
    };
    onMessageStruct messageStruct;

    // Queues containing above structs
    queue<onMessageStruct> queueInternal;

    GroundSteeringReading msgSteering;
    PedalPositionReading msgPedal;

    // ****** Internal communication ****** //

    auto onRemoteModeMessage = [&mode](cluon::data::Envelope &&env){
        RemoteModeMessage msg = cluon::extractMessage<RemoteModeMessage>(move(env));
        cout << "Setting mode to " << endl;
        if (msg.mode())
        {
            cout << "LEADER" << endl;
        }
        else
        {
            cout << "FOLLOWER" << endl;
        }
        mode = msg.mode();
    };
    messageStruct.delegate = onRemoteModeMessage;
    messageStruct.messageIdentifier = RemoteModeMessage::ID();
    queueInternal.push(messageStruct);

    auto onPedalPositionReadingMessage = [&mode, &msgPedal, &od4PwmOds](cluon::data::Envelope &&env){
        PedalPositionReading msg = cluon::extractMessage<PedalPositionReading>(move(env));
        cout << "Received pedal position reading " << msg.percent() << endl;
        if(emergencyBreakreak == false)
        {
            cout << "The vehicle will not move - obstacle detected within 10 cm " << endl;
            //msgPedal.percent(0);
            //od4PwmOds.send(msgPedal);
            return;
        }

        if(mode == LEADER)
        {
            cout << "LEADER MODE ACTIVATED, forwarding pedal reading to pwm/ods " << endl;
            msgPedal.percent(msg.percent());
            od4PwmOds.send(msgPedal);
        }
        else
        {
            cout << "LEADER DEACTIVATED, ignoring to forward pedal reading" << endl;
        }
    };

    messageStruct.delegate = onPedalPositionReadingMessage;
    messageStruct.messageIdentifier = PedalPositionReading::ID();
    queueInternal.push(messageStruct);

    auto onGroundSteeringReadingMessage = [&mode, &msgSteering, &od4PwmOds](cluon::data::Envelope &&env){
        GroundSteeringReading msg = cluon::extractMessage<GroundSteeringReading>(move(env));
        cout << "Received ground steering reading " << msg.steeringAngle() << endl;
        if(mode == LEADER)
        {
            cout << "LEADER MODE ACTIVATED, forwarding steering angle to pwm/ods " << endl;
            msgSteering.steeringAngle(msg.steeringAngle());
            od4PwmOds.send(msgSteering);
        }
        else
        {
            cout << "LEADER DEACTIVATED, ignoring to forward steering angle" << endl;
        }
    };

    messageStruct.delegate = onGroundSteeringReadingMessage;
    messageStruct.messageIdentifier = GroundSteeringReading::ID();
    queueInternal.push(messageStruct);

    auto onUltrasonicReadingMessage = [&mode, &msgSteering, &msgPedal, &od4PwmOds](cluon::data::Envelope &&env){
        DistanceReading msg = cluon::extractMessage<DistanceReading>(move(env));
        if(msg.distance() <= 10 && emergencyBreak == false)
        {
            cout << "STOPS! Ultrasonic reading below 10 cm: " << msg.distance() << endl;
            msgSteering.steeringAngle(0);
            msgPedal.percent(0);
            od4PwmOds.send(msgSteering);
            od4PwmOds.send(msgPedal);
        }
        else if(msg.distance() > 10 && emergencyBreak == true)
        {
            emergencyBreak = false;
        }
    };

    messageStruct.delegate = onGroundSteeringReadingMessage;
    messageStruct.messageIdentifier = GroundSteeringReading::ID();
    queueInternal.push(messageStruct);

    //TODO add functions to carry out when V2V messages are received, keep emergencyBreak in mind

    // Register the lambda functions and message identifiers for each instance in queue
    while (!queueInternal.empty())
    {
        od4Internal.dataTrigger(queueInternal.front().messageIdentifier, queueInternal.front().delegate);
        queueInternal.pop();
    }


    // ****** Autonomous driving code ****** //
    //TODO add code for autonomous following, keep emergencyBreak in mind

    cerr << "Maneuvering script v0.1" << endl;

    while (od4PwmOds.isRunning() && od4Internal.isRunning())
    {

    }

    return 0;
}