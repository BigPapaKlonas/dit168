// Vera requires this "Copyright" notice

#include "maneuvering.hpp"

#define FOLLOWER 0
#define LEADER 1

using namespace opendlv::proxy;

void followSteering(std::queue<float>, float offset, int verbose);

int main(int argc, char **argv)
{
    // Variables used throughout the code
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    uint16_t cidPwmOds;
    uint16_t cidInternal;
    int mode = LEADER;
    float offset = 0;
    bool emergencyBreak = false;
    int emergencyBreakDistance = 0;
    unsigned long queueDelay = 0;
    int verbose = 0;

    // Messages objects used for controlling the car's engine and steering
    GroundSteeringReading msgSteering;
    PedalPositionReading msgPedal;

    // Checks that command line arguments are provided
    if (commandlineArguments.count("cid_pwm_ods") == 0 ||
        commandlineArguments.count("cid_internal") == 0 ||
        commandlineArguments.count("offset") == 0 ||
        commandlineArguments.count("emergency_break_distance") == 0 ||
        commandlineArguments.count("queue_delay") == 0 ||
        commandlineArguments.count("verbose") == 0)
    {
        std::cerr <<"You must provide command line arguments for CIDs, offset (0-1), "
                "emergency break distance (0-57, in cm) and amount of steering messages received "
                "before execution of steering commands"  << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid_pwm_ods=120 --cid_internal=122 --offset=0.5 "
                "--emergency_break_distance=10 --queue_delay=10 --verbose=1" << std::endl;
        return -1;
    }
    else
    {
        cidPwmOds = stoi(commandlineArguments["cid_pwm_ods"]);
        cidInternal = stoi(commandlineArguments["cid_internal"]);
        offset = stof(commandlineArguments["offset"]);
        emergencyBreakDistance = stoi(commandlineArguments["emergency_break_distance"]);
        queueDelay = stoi(commandlineArguments["queue_delay"]);
        verbose = stoi(commandlineArguments["verbose"]);
    }

    // Checks data provided from command line arguments
    if (cidInternal < 120 || cidInternal > 129 || cidPwmOds < 120 || cidPwmOds > 129)
    {
        std::cerr << "The OpenDaVINCI session identifiers (CIDs) must be in the range 120 to 129"
                  << std::endl;
        return -1;
    }

    if (cidInternal == cidPwmOds)
    {
        std::cerr << "The two OpenDaVINCI session identifiers (CIDs) must not be the same"
                  << std::endl;
        return -1;
    }

    if (offset < 0 || offset > 1)
    {
        std::cerr << "The offset must be in the interval 0-1"
                  << std::endl;
        return -1;
    }

    if (emergencyBreakDistance < 0 || emergencyBreakDistance > 57)
    {
        std::cerr << "The emergency break distance must be in the interval 0-57"
                  << std::endl;
        return -1;
    }

    // Initializing od4 session for odsupercomponent/pwm-motor and internal communication
    od4PwmOds = std::make_shared<cluon::OD4Session>(cidPwmOds);
    cluon::OD4Session od4Internal(cidInternal);

    // Check that the od4 sessions are running
    if (od4PwmOds->isRunning() == 0)
    {
        std::cerr << "ERROR: No OD4 responsible for the pwm-motor odsupercomponent is running!"
                  << std::endl;
        return -1;
    }
    else if (od4Internal.isRunning() == 0)
    {
        std::cerr << "ERROR: No OD4 responsible for the internal communication is running!"
                  << std::endl;
        return -1;
    }

    // Struct with function, delegate, to be called when message, messageIdentifier, is received
    struct onMessageStruct
    {
        int32_t messageIdentifier = 0;
        std::function< void(cluon::data::Envelope &&envelope)> delegate;
    };
    onMessageStruct messageStruct;

    // Queues containing above structs
    std::queue<onMessageStruct> queueInternal;

    /*************************************************************
    *********** Set-up of threads on message receiving ***********
    **************************************************************/

    /**************** Remote controller messages *****************/
    auto onRemoteModeMessage = [&mode, &verbose](cluon::data::Envelope &&env)
    {
        Role msg = cluon::extractMessage<Role>(std::move(env));
        if (msg.current() == LEADER)
        {
            if (verbose)
            {
                std::cout << "Changing mode to LEADER" << std::endl;
            }
            mode = msg.current();
        }
        else if (msg.current() == FOLLOWER)
        {
            if (verbose)
            {
                std::cout << "Changing mode to FOLLOWER" << std::endl;
            }
            mode = msg.current();
        }
        else
        {
            std::cerr << "Invalid mode received on internal od4 channel.. Ignoring the message"
                      << std::endl;
        }
    };
    messageStruct.delegate = onRemoteModeMessage;
    messageStruct.messageIdentifier = Role::ID();
    queueInternal.push(messageStruct);

    auto onPedalPositionReadingMessage = [&offset, &mode, &msgPedal, &msgSteering, &emergencyBreak,
            &emergencyBreakDistance, &verbose] (cluon::data::Envelope &&env)
    {
        PedalPositionReading msg = cluon::extractMessage<PedalPositionReading>(std::move(env));
        if (emergencyBreak == true)
        {
            if (verbose)
            {
                std::cout << "The vehicle will not move - obstacle detected within "
                          << emergencyBreakDistance << " cm"<< std::endl;
            }
            return;
        }

        if (mode == LEADER)
        {
            if (verbose)
            {
                std::cout << "Forwarding pedal position reading: " << msg.percent()
                          << " to motor proxy" << std::endl;
            }

            msgSteering.steeringAngle(-offset); // Applies offset to account for vehicle's drift
            od4PwmOds->send(msgSteering);
            msgPedal.percent(msg.percent());
            od4PwmOds->send(msgPedal);
        }
        else
        {
            if (verbose)
            {
                std::cout << "Not in leader mode.. Ignoring received pedal position reading"
                          << std::endl;
            }
        }
    };
    messageStruct.delegate = onPedalPositionReadingMessage;
    messageStruct.messageIdentifier = PedalPositionReading::ID();
    queueInternal.push(messageStruct);

    auto onGroundSteeringReadingMessage = [&mode, &msgSteering, &verbose]
            (cluon::data::Envelope &&env)
    {
        GroundSteeringReading msg = cluon::extractMessage<GroundSteeringReading>(std::move(env));
        if (mode == LEADER)
        {
            if (verbose)
            {
                std::cout << "Forwarding steering angle: " << msg.steeringAngle()
                          << " to odsupercomponent" << std::endl;
            }
            msgSteering.steeringAngle(msg.steeringAngle());
            od4PwmOds->send(msgSteering);
        }
        else
        {
            if (verbose)
            {
                std::cout << "Not in leader mode, ignoring received ground steering angle"
                          << std::endl;
            }
        }
    };
    messageStruct.delegate = onGroundSteeringReadingMessage;
    messageStruct.messageIdentifier = GroundSteeringReading::ID();
    queueInternal.push(messageStruct);

    // *********** Sensor messages ************ //
    auto onUltrasonicReadingMessage = [&mode, &msgSteering, &msgPedal, &emergencyBreak,
            &emergencyBreakDistance, &verbose] (cluon::data::Envelope &&env)
    {
        DistanceReading msg = cluon::extractMessage<DistanceReading>(std::move(env));
        if (msg.distance()*100 <= emergencyBreakDistance && emergencyBreak == false)
        {
            emergencyBreak = true;
            msgPedal.percent(0);
            od4PwmOds->send(msgPedal);
            if (verbose)
            {
                std::cout << "STOPS! Ultrasonic reading: " << msg.distance()*100 << " cm"
                          << std::endl;
            }
        }
        else if (msg.distance()*100 > emergencyBreakDistance && emergencyBreak == true)
        {
            emergencyBreak = false;
        }
    };
    messageStruct.delegate = onUltrasonicReadingMessage;
    messageStruct.messageIdentifier = DistanceReading::ID();
    queueInternal.push(messageStruct);

    // ************ V2V messages ************** //
    std::queue <float> steeringQueue;

    auto onLeaderStatus = [&mode, &steeringQueue, &offset, &queueDelay, &verbose, &msgPedal]
            (cluon::data::Envelope &&env)
    {
        LeaderStatus msg = cluon::extractMessage<LeaderStatus>(std::move(env));
        if (mode == FOLLOWER)
        {
            if (verbose)
            {
                std::cout << "Forwarding pedal position reading: " << msg.speed()
                          << " from LeaderStatus to motor proxy" << std::endl;
            }

            msgPedal.percent(msg.speed());
            od4PwmOds->send(msgPedal);
            
            if (msg.speed() != 0 && steeringQueue.size() < queueDelay)
            {
                steeringQueue.push(msg.steeringAngle());
            }
            else if (msg.speed() != 0 && steeringQueue.size() >= queueDelay)
            {
                steeringQueue.push(msg.steeringAngle());
                followSteering(steeringQueue, offset, verbose);
                steeringQueue.pop();
            }
        }
        else
        {
            if (verbose)
            {
                std::cout << "Not in follower mode, ignoring received LeaderStatus" << std::endl;
            }
        }
    };
    messageStruct.delegate = onLeaderStatus;
    messageStruct.messageIdentifier = LeaderStatus::ID();
    queueInternal.push(messageStruct);

    // Register the lambda functions and message identifiers for each instance in queue
    while (queueInternal.empty() == false)
    {
        od4Internal.dataTrigger(queueInternal.front().messageIdentifier,
                                queueInternal.front().delegate);
        queueInternal.pop();
    }
    /***** End of message receiving set-up *****/


    std::cout << "Maneuvering script version 1.0 successfully started" << std::endl;
    while (od4PwmOds->isRunning() && od4Internal.isRunning()) {}

    return 0;
}

/**
 * Steering logic for follow leader car
 * @param queue
 * @param offset
 * @param verbose
 */
void followSteering(std::queue<float> queue, float offset, int verbose)
{
    GroundSteeringReading msgSteering;
    float steering = queue.front();

    if (steering == 0) // If the LeaderStatus contains GroundSteeringReading 0, offset is applied
    {
        steering = -offset;
    }

    if (verbose)
    {
        std::cout << "Forwarding pedal position reading: " << queue.front() << " from LeaderStatus "
                "to odsupercomponent" << std::endl;
    }

    msgSteering.steeringAngle(steering);
    od4PwmOds->send(msgSteering);
}
