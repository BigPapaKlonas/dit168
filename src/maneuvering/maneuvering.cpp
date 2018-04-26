// Vera requires this "Copyright" notice

#include "maneuvering.h"

#define FOLLOWER 0
#define LEADER 1

using namespace opendlv::proxy;

uint64_t getTime();

void follow (std::queue <LeaderStatus>, float offset, int queueDelay);
void follow(uint64_t timestamp, float speed, float steeringAngle, uint8_t distanceTraveled, float offset);

int main(int argc, char **argv)
{

    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    uint16_t cidPwmOds;
    uint16_t cidInternal;
    uint16_t mode = LEADER;
    float offset = 0;
    bool emergencyBreak = false;
    int emergencyBreakDistance = 0;
    int queueDelay = 0;

    // In case no CID is provided
    if (commandlineArguments.count("cid_pwm_ods") == 0 || commandlineArguments.count("cid_internal") == 0
            || commandlineArguments.count("offset") == 0 || commandlineArguments.count("emergency_break_distance") == 0
                                                            || commandlineArguments.count("queue_delay") == 0)
    {
        std::cerr <<"You must specify which OpenDaVINCI session identifiers (CIDs) the "
                    "maneuvering script shall listen to! And the offset! And emergency break distance in cm"
                "and the amount of messages needed in queue before executing commands"  << std::endl;
        std::cerr <<"cid_pwn_ods for the pwm-motor and odsupercomponent microservice\n"
                    "and cid_internal for the internal communication CID"<< std::endl;
        std::cerr << "Example: " << argv[0] << " --cid_pwm_ods=120 --cid_internal=122 --offset=0.5 "
                "--emergency_break_distance=10 --queue_delay=10" << std::endl;
        return -1;
    }
    else
    {
        cidPwmOds = stoi(commandlineArguments["cid_pwm_ods"]);
        cidInternal = stoi(commandlineArguments["cid_internal"]);
        offset = stof(commandlineArguments["offset"]);
        emergencyBreakDistance = stoi(commandlineArguments["emergency_break_distance"]);
        queueDelay = stoi(commandlineArguments["queue_delay"]);
    }

    if (cidInternal < 120 || cidInternal > 129 || cidPwmOds < 120 || cidPwmOds > 129)
    {
        std::cerr << "The OpenDaVINCI session identifiers (CIDs) must be in the range 120 to 129"
                  << std::endl;
        return -1;
    }

    // Checks CID range
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


    // Initializing od4 session for odsupercomponent/pwm-motor and internal communication
    od4PwmOds = std::make_shared<cluon::OD4Session>(cidPwmOds);
    cluon::OD4Session od4Internal(cidInternal);

    // Check that the od4 sessions are running
     if (od4PwmOds->isRunning() == 0)
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
    std::queue<onMessageStruct> queueInternal;

    GroundSteeringReading msgSteering;
    PedalPositionReading msgPedal;

    // ****** Remote controller messages ****** //
    auto onRemoteModeMessage = [&mode](cluon::data::Envelope &&env){
        Role msg = cluon::extractMessage<Role>(std::move(env));
        std::cout << "Setting mode to " << std::endl;
        if (msg.current())
        {
            std::cout << "LEADER" << std::endl;
        }
        else
        {
            std::cout << "FOLLOWER" << std::endl;
        }
        mode = msg.current();
    };
    messageStruct.delegate = onRemoteModeMessage;
    messageStruct.messageIdentifier = Role::ID();
    queueInternal.push(messageStruct);

    auto onPedalPositionReadingMessage = [&offset, &mode, &msgPedal, &msgSteering, &emergencyBreak, &emergencyBreakDistance]
            (cluon::data::Envelope &&env){
        PedalPositionReading msg = cluon::extractMessage<PedalPositionReading>(std::move(env));
        std::cout << "Received pedal position reading " << msg.percent() << std::endl;
        if(emergencyBreak == true)
        {
            std::cout << "The vehicle will not move - obstacle detected within " << emergencyBreakDistance << " cm"<< std::endl;
            return;
        }

        if(mode == LEADER)
        {
            std::cout << "LEADER MODE ACTIVATED, forwarding pedal reading to pwm/ods " << std::endl;
            msgSteering.steeringAngle(-offset);
            od4PwmOds->send(msgSteering);
            msgPedal.percent(msg.percent());
            od4PwmOds->send(msgPedal);

        }
        else
        {
            std::cout << "LEADER DEACTIVATED, ignoring to forward pedal reading" << std::endl;
        }
    };
    messageStruct.delegate = onPedalPositionReadingMessage;
    messageStruct.messageIdentifier = PedalPositionReading::ID();
    queueInternal.push(messageStruct);

    auto onGroundSteeringReadingMessage = [&mode, &msgSteering](cluon::data::Envelope &&env){
        GroundSteeringReading msg = cluon::extractMessage<GroundSteeringReading>(std::move(env));
        std::cout << "Received ground steering reading " << msg.steeringAngle() << std::endl;
        if(mode == LEADER)
        {
            std::cout << "LEADER MODE ACTIVATED, forwarding steering angle to pwm/ods " << std::endl;
            msgSteering.steeringAngle(msg.steeringAngle());
            od4PwmOds->send(msgSteering);
        }
        else
        {
            std::cout << "LEADER DEACTIVATED, ignoring to forward steering angle" << std::endl;
        }
    };
    messageStruct.delegate = onGroundSteeringReadingMessage;
    messageStruct.messageIdentifier = GroundSteeringReading::ID();
    queueInternal.push(messageStruct);


    // *********** Sensor messages ************ //
    auto onUltrasonicReadingMessage = [&mode, &msgSteering, &msgPedal, &emergencyBreak, &emergencyBreakDistance]
            (cluon::data::Envelope &&env){
        DistanceReading msg = cluon::extractMessage<DistanceReading>(std::move(env));
        if(msg.distance()*100 <= emergencyBreakDistance && emergencyBreak == false)
        {
            emergencyBreak = true;
            msgPedal.percent(0);
            od4PwmOds->send(msgPedal);
            std::cout << "STOPS! Ultrasonic reading: " << msg.distance()*100 << " cm" << std::endl;
        }
        else if(msg.distance()*100 > emergencyBreakDistance && emergencyBreak == true)
        {
            emergencyBreak = false;
        }
    };
    messageStruct.delegate = onUltrasonicReadingMessage;
    messageStruct.messageIdentifier = DistanceReading::ID();
    queueInternal.push(messageStruct);

    // ************ V2V messages ************** //
    std::queue <LeaderStatus> leaderStatusQueue;

    auto onLeaderStatus = [&mode, &leaderStatusQueue, &offset, &queueDelay]
            (cluon::data::Envelope &&env){
        LeaderStatus msg = cluon::extractMessage<LeaderStatus>(std::move(env));
        std::cout << "LeaderStatus received with timestamp: " << msg.timestamp() << std::endl;
        if(mode == FOLLOWER)
        {
            follow(msg.timestamp(), msg.speed(), msg.steeringAngle(), msg.distanceTraveled(), offset);

            /*
            if(msg.speed() != 0)
            {
                leaderStatusQueue.push(msg);
                follow(leaderStatusQueue, offset, queueDelay);
            }
             **/
        }
    };
    messageStruct.delegate = onLeaderStatus;
    messageStruct.messageIdentifier = LeaderStatus::ID();
    queueInternal.push(messageStruct);

    // Register the lambda functions and message identifiers for each instance in queue
    while (!queueInternal.empty())
    {
        od4Internal.dataTrigger(queueInternal.front().messageIdentifier, queueInternal.front().delegate);
        queueInternal.pop();
    }


    std::cerr << "Maneuvering script v0.2" << std::endl;
    while (od4PwmOds->isRunning() && od4Internal.isRunning())
    {

    }

    return 0;
}

/**
 * Function handles the autonomous following logic
 * @param queue
 * @param offset
 */
void follow (std::queue <LeaderStatus> queue, float offset, int queueDelay)
{
    if(queue.size() <= queueDelay)
    {
        std::cout << "Less than " << queueDelay << " messages in queue. Only: " << queue.size() << std::endl;
        return;
    }

    std::cout << "Following " << std::endl;

    GroundSteeringReading msgSteering;
    PedalPositionReading msgPedal;
    float steering = queue.front().steeringAngle();

    if (steering == 0)
        steering = -offset;

    msgSteering.steeringAngle(steering);
    od4PwmOds->send(msgSteering);

    msgPedal.percent(queue.front().speed());
    od4PwmOds->send(msgPedal);

    queue.pop();
}

/**
 *
 * @param timestamp
 * @param speed
 * @param steeringAngle
 * @param distanceTraveled
 * @param offset
 */
void follow(uint64_t timestamp, float speed, float steeringAngle, uint8_t distanceTraveled, float offset)
{

    std::cout << "Following " << std::endl;

    GroundSteeringReading msgSteering;
    PedalPositionReading msgPedal;

    float steering = steeringAngle;

    if (steering == 0)
         steering = -offset;

    msgSteering.steeringAngle(steering);
    od4PwmOds->send(msgSteering);

    msgPedal.percent(speed);
    od4PwmOds->send(msgPedal);
}


/**
* Gets current time in milliseconds.
*
* @return current time in milliseconds
*/
uint64_t getTime()
{
    timeval time;
    gettimeofday(&time, nullptr);
    return (uint64_t ) time.tv_usec / 1000;
}